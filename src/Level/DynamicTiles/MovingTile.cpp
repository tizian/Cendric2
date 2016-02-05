#include "Level/DynamicTiles/MovingTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Moving, MovingTile)

void MovingTile::setMovingTileData(const MovingTileData& data) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, data.length * TILE_SIZE_F, TILE_SIZE_F));
	float phi = degToRad(static_cast<float>(data.initialDirection));

	m_currentVelocity.x = data.speed * std::cos(phi); 
	m_currentVelocity.y = data.speed * std::sin(phi);

	m_distanceTime = data.speed == 0 ? sf::Time::Zero : sf::seconds(static_cast<float>(data.distance) / static_cast<float>(data.speed));
	m_timeUntilTurn = m_distanceTime;

	m_isOnHold = false;
	setFrozen(data.isFrozen);
}

void MovingTile::init() {
	m_isAlwaysUpdate = true;
}

void MovingTile::loadAnimation(int skinNr) {

	int length = static_cast<int>(m_boundingBox.width / TILE_SIZE_F);

	sf::Sprite sprite;
	sprite.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_tile_moving));

	if (length == 1) {
		sprite.setTextureRect(sf::IntRect(150, skinNr * 2 * TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
		m_normalSprites.push_back(sprite);

		sprite.setTextureRect(sf::IntRect(150, 0, TILE_SIZE, 2 * TILE_SIZE));
		m_frozenSprites.push_back(sprite);
		return;
	}

	sprite.setTextureRect(sf::IntRect(0, skinNr * 2 * TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
	m_normalSprites.push_back(sprite);

	sprite.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, 2 * TILE_SIZE));
	m_frozenSprites.push_back(sprite);
	length--;

	while (length > 1) {
		sprite.setTextureRect(sf::IntRect(50, skinNr * 2 * TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
		m_normalSprites.push_back(sprite);

		sprite.setTextureRect(sf::IntRect(50, 0, TILE_SIZE, 2 * TILE_SIZE));
		m_frozenSprites.push_back(sprite);
		length--;
	}

	sprite.setTextureRect(sf::IntRect(100, skinNr * 2 * TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE));
	m_normalSprites.push_back(sprite);

	sprite.setTextureRect(sf::IntRect(100, 0, TILE_SIZE, 2 * TILE_SIZE));
	m_frozenSprites.push_back(sprite);
}

void MovingTile::update(const sf::Time& frameTime) {
	if (!(m_isFrozen || m_isOnHold)) {
		updateTime(m_timeUntilTurn, frameTime);
		if (m_timeUntilTurn == sf::Time::Zero) {
			m_timeUntilTurn = m_distanceTime;
			m_currentVelocity = -m_currentVelocity;
			setRelativeVelocity(m_currentVelocity);
		}
		MovableGameObject::update(frameTime);
	}
}

void MovingTile::render(sf::RenderTarget& target) {
	for (auto& sprite : m_normalSprites) {
		target.draw(sprite);
	}
	if (m_isFrozen) {
		for (auto& sprite : m_frozenSprites) {
			target.draw(sprite);
		}
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
}

GameObjectType MovingTile::getConfiguredType() const {
	return GameObjectType::_DynamicTile;
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

void MovingTile::setFrozen(bool frozen) {
	m_isFrozen = frozen;
	setRelativeVelocity(m_isOnHold ? sf::Vector2f() : m_currentVelocity);
	setPosition(getPosition());
}