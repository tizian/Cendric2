#include "Level/DynamicTiles/MovingTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Moving, MovingTile)

const sf::Time MovingTile::FROZEN_TIME = sf::seconds(10.f);
const sf::Time MovingTile::FROZEN_FADING_TIME = sf::seconds(2.f);
const std::string MovingTile::SPRITE_PATH = "res/assets/level_dynamic_tiles/spritesheet_tiles_moving.png";

MovingTile::MovingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen),
	LevelMovableTile(levelScreen),
	LeverDependentTile(levelScreen) {
	m_movingParent = this;
	m_relativeVelocity.x = 0.f;
	m_relativeVelocity.y = 0.f;
}

MovingTile::~MovingTile() {
	delete m_spikes;
}

void MovingTile::setMovingTileData(const MovingTileData& data) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, data.length * TILE_SIZE_F, 40.f));
	float phi = degToRad(static_cast<float>(data.initialDirection - 90));

	m_currentVelocity.x = std::round(data.speed * std::cos(phi));
	m_currentVelocity.y = std::round(data.speed * std::sin(phi));

	m_distanceTime = data.speed == 0 ? sf::Time::Zero : sf::seconds(static_cast<float>(data.distance) / static_cast<float>(data.speed));
	m_timeUntilTurn = m_distanceTime;

	setFrozen(data.isFrozen, true);
	setInitialState(data.isActive);

	m_isOneWay = data.isOneWay;

	if (data.spikesTop || data.spikesBottom) {
		delete m_spikes;
		m_spikes = new MovingTileSpikes(data.spikesTop, data.spikesBottom, data.length, m_mainChar);
	}
}

void MovingTile::init() {
	m_isCollidable = true;
	m_isStrictlyCollidable = true;
}

void MovingTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int length = static_cast<int>(m_boundingBox.width / TILE_SIZE_F);
	int frozenTileY = 3 * TILE_SIZE;
	int tileY = 5 * TILE_SIZE + skinNr * 2 * TILE_SIZE;

	sf::Sprite sprite;
	sprite.setTexture(*tex);

	if (length == 1) {
		sprite.setTextureRect(sf::IntRect(150, tileY, TILE_SIZE, 2 * TILE_SIZE));
		m_normalSprites.push_back(sprite);

		sprite.setTextureRect(sf::IntRect(150, frozenTileY, TILE_SIZE, 2 * TILE_SIZE));
		m_frozenSprites.push_back(sprite);
		return;
	}

	sprite.setTextureRect(sf::IntRect(0, tileY, TILE_SIZE, 2 * TILE_SIZE));
	m_normalSprites.push_back(sprite);

	sprite.setTextureRect(sf::IntRect(0, frozenTileY, TILE_SIZE, 2 * TILE_SIZE));
	m_frozenSprites.push_back(sprite);
	length--;

	while (length > 1) {
		sprite.setTextureRect(sf::IntRect(50, tileY, TILE_SIZE, 2 * TILE_SIZE));
		m_normalSprites.push_back(sprite);

		sprite.setTextureRect(sf::IntRect(50, frozenTileY, TILE_SIZE, 2 * TILE_SIZE));
		m_frozenSprites.push_back(sprite);
		length--;
	}

	sprite.setTextureRect(sf::IntRect(100, tileY, TILE_SIZE, 2 * TILE_SIZE));
	m_normalSprites.push_back(sprite);

	sprite.setTextureRect(sf::IntRect(100, frozenTileY, TILE_SIZE, 2 * TILE_SIZE));
	m_frozenSprites.push_back(sprite);
}

void MovingTile::update(const sf::Time& frameTime) {
	if (m_isFrozen) {
		if (!m_isPermanentlyFrozen) {
			updateTime(m_frozenTime, frameTime);
		}
		if (m_frozenTime == sf::Time::Zero) {
			setFrozen(false);
			return;
		}
		// scale frozen sprites accordingly for fading
		float scale = m_frozenTime > FROZEN_FADING_TIME ? 1.f : m_frozenTime / FROZEN_FADING_TIME;
		sf::Uint8 scaleU = static_cast<sf::Uint8>(scale * 255);
		for (auto& sprite : m_frozenSprites) {
			sprite.setColor(sf::Color(255, 255, 255, scaleU));
		}
		return;
	}
	if (m_isActive) {
		updateTime(m_timeUntilTurn, frameTime);
		if (m_timeUntilTurn == sf::Time::Zero) {
			m_timeUntilTurn = m_distanceTime;
			m_currentVelocity = -m_currentVelocity;
			m_relativeVelocity = m_currentVelocity;
		}
		MovableGameObject::update(frameTime);
	}

	if (m_spikes) {
		m_spikes->update(frameTime);
	}
}

void MovingTile::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_isOneWay) {
		MovableGameObject::updateRelativeVelocity(frameTime);
		return;
	}
	else {
		LevelMovableTile::updateRelativeVelocity(frameTime);
	}
}

void MovingTile::render(sf::RenderTarget& target) {
	if (m_spikes) {
		m_spikes->renderBottom(target);
	}
	for (auto& sprite : m_normalSprites) {
		target.draw(sprite);
	}
	if (m_isFrozen) {
		for (auto& sprite : m_frozenSprites) {
			target.draw(sprite);
		}
	}
	if (m_spikes) {
		m_spikes->renderTop(target);
	}
}

void MovingTile::setPosition(const sf::Vector2f& position) {
	MovableGameObject::setPosition(position);
	float offset = 0.f;
	for (auto& sprite : m_normalSprites) {
		sprite.setPosition(getPosition().x + offset, getPosition().y - TILE_SIZE);
		offset += TILE_SIZE;
	}
	if (m_isFrozen) {
		offset = 0.f;
		for (auto& sprite : m_frozenSprites) {
			sprite.setPosition(getPosition().x + offset, getPosition().y - TILE_SIZE);
			offset += TILE_SIZE;
		}
	}
	if (m_spikes) {
		m_spikes->setPosition(position);
	}
}

const sf::Vector2f& MovingTile::getRelativeVelocity() const {
	return m_relativeVelocity;
}

void MovingTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::IceBall:
		setFrozen(true);
		spell->setDisposed();
		break;
	case SpellID::FireBall:
		setFrozen(false);
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void MovingTile::setFrozen(bool frozen, bool permanent) {
	m_isFrozen = frozen;
	m_isPermanentlyFrozen = permanent;
	m_relativeVelocity = m_isFrozen || !m_isActive ? sf::Vector2f() : m_currentVelocity;
	setPosition(getPosition());
	if (m_isFrozen) {
		m_frozenTime = FROZEN_TIME;
	}
}

void MovingTile::setInitialState(bool on) {
	m_isActive = on;
	m_relativeVelocity = m_isFrozen || !m_isActive ? sf::Vector2f() : m_currentVelocity;
	setPosition(getPosition());
}

void MovingTile::switchTile() {
	setInitialState(!m_isActive);
}

bool MovingTile::isSwitchable() const {
	return true;
}

std::string MovingTile::getSpritePath() const {
	return SPRITE_PATH;
}

/////////////////////////////////////////////////////////////////////
//// MOVING TILE SPIKES /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

MovingTileSpikes::MovingTileSpikes(bool top, bool bottom, int size, LevelMainCharacter* mainChar) {
	m_mainChar = mainChar;

	float bbHeight;
	if (top && bottom) {
		bbHeight = TILE_SIZE_F + 2 * 20.f;
		m_boundingBoxOffset = -20.f;
	}
	else {
		bbHeight = 20.f;
		m_boundingBoxOffset = bottom ? TILE_SIZE_F : -20.f;
	}

	m_boundingBox = sf::FloatRect(0.f, 0.f, size * TILE_SIZE_F, bbHeight);

	loadAnimation(top, bottom, size);
}

void MovingTileSpikes::loadAnimation(bool top, bool bottom, int size) {
	sf::Texture& tex = *g_resourceManager->getTexture(MovingTile::SPRITE_PATH);
	sf::Sprite sprite;

	sprite.setTexture(tex);

	if (size == 1) {
		if (top) {
			sprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
			m_topSprites.push_back(sprite);
		}

		if (bottom) {
			sprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
			m_bottomSprites.push_back(sprite);
		}
		return;
	}

	if (top) {
		sprite.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
		m_topSprites.push_back(sprite);
	}

	if (bottom) {
		sprite.setTextureRect(sf::IntRect(0, TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
		m_bottomSprites.push_back(sprite);
	}

	size--;

	while (size > 1) {
		if (top) {
			sprite.setTextureRect(sf::IntRect(50, 0, TILE_SIZE, TILE_SIZE));
			m_topSprites.push_back(sprite);
		}

		if (bottom) {
			sprite.setTextureRect(sf::IntRect(50, TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
			m_bottomSprites.push_back(sprite);
		}
		size--;
	}

	if (top) {
		sprite.setTextureRect(sf::IntRect(100, 0, TILE_SIZE, TILE_SIZE));
		m_topSprites.push_back(sprite);
	}

	if (bottom) {
		sprite.setTextureRect(sf::IntRect(100, TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
		m_bottomSprites.push_back(sprite);
	}
}

void MovingTileSpikes::update(const sf::Time& frameTime) {
	if (m_mainChar->isDead()) return;
	if (m_mainChar->getBoundingBox()->intersects(m_boundingBox)) {
		m_mainChar->setDead();
	}
}

void MovingTileSpikes::renderTop(sf::RenderTarget& target) {
	for (auto& sprite : m_topSprites) {
		target.draw(sprite);
	}
}

void MovingTileSpikes::renderBottom(sf::RenderTarget& target) {
	for (auto& sprite : m_bottomSprites) {
		target.draw(sprite);
	}
}

void MovingTileSpikes::setPosition(const sf::Vector2f& position) {
	float x = 0.f;
	for (auto& sprite : m_topSprites) {
		sprite.setPosition(sf::Vector2f(position.x + x, position.y -TILE_SIZE_F));
		x += TILE_SIZE_F;
	}

	x = 0.f;
	for (auto& sprite : m_bottomSprites) {
		sprite.setPosition(sf::Vector2f(position.x + x, position.y));
		x += TILE_SIZE_F;
	}

	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y + m_boundingBoxOffset;
}
