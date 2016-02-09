#include "Level/DynamicTiles/MovingTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Moving, MovingTile)

MovingTile::MovingTile(Level* level) : LevelDynamicTile(level) { 
	m_movingParent = this; 
	m_relativeVelocity.x = 0.f;
	m_relativeVelocity.y = 0.f;
}

void MovingTile::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_movingParent == nullptr) return;
	sf::Vector2f oldPos = m_position;
	MovableGameObject::updateRelativeVelocity(frameTime);
	sf::Vector2f posDiff = m_position - oldPos;

	sf::FloatRect newBoundingBoxX = m_boundingBox;
	newBoundingBoxX.top -= posDiff.y;

	sf::FloatRect newBoundingBoxY = m_boundingBox;
	newBoundingBoxY.left -= posDiff.x;

	// check if we hit the other movable objects that do not have the same parent as us. we have precedence and shift other objects away.
	auto mainChars = m_screen->getObjects(GameObjectType::_LevelMainCharacter);
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto movableTiles = m_screen->getObjects(GameObjectType::_MovableTile);

	LevelMovableGameObject* mainChar = dynamic_cast<LevelMovableGameObject*>((*mainChars)[0]);
	if (mainChar->getMovingParent() != getMovingParent() && !mainChar->isDead()) {
		const sf::FloatRect& mainCharBB = *mainChar->getBoundingBox();
		if (epsIntersect(mainCharBB, newBoundingBoxX)) {
			mainChar->setPositionX(mainChar->getPosition().x + posDiff.x);
			mainChar->lockRelativeVelocityX();
		} 
		if (epsIntersect(mainCharBB, newBoundingBoxY)) {
			mainChar->setPositionY(mainChar->getPosition().y + posDiff.y);
			mainChar->lockRelativeVelocityY();
		}
	}

	for (auto enemy : *enemies) {
		LevelMovableGameObject* mob = dynamic_cast<LevelMovableGameObject*>(enemy);
		if (mob->getMovingParent() != getMovingParent() && !mob->isDead()) {
			const sf::FloatRect& mobBB = *mob->getBoundingBox();
			if (epsIntersect(mobBB, newBoundingBoxX)) {
				mob->setPositionX(mob->getPosition().x + posDiff.x);
				mob->lockRelativeVelocityX();
			}
			if (epsIntersect(mobBB, newBoundingBoxY)) {
				mob->setPositionY(mob->getPosition().y + posDiff.y);
				mob->lockRelativeVelocityY();
			}
		}
	}

	for (auto movableTile : *movableTiles) {
		MovableGameObject* mob = dynamic_cast<MovableGameObject*>(movableTile);
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(movableTile);
		if (mob->getMovingParent() != getMovingParent() && tile->getDynamicTileID() != LevelDynamicTileID::Moving && tile->getIsCollidable()) {
			const sf::FloatRect& mobBB = *mob->getBoundingBox();
			if (epsIntersect(mobBB, newBoundingBoxX)) {
				mob->setPositionX(mob->getPosition().x + posDiff.x);
				mob->lockRelativeVelocityX();
			}
			if (epsIntersect(mobBB, newBoundingBoxY)) {
				mob->setPositionY(mob->getPosition().y + posDiff.y);
				mob->lockRelativeVelocityY();
			}
		}
	}
}

void MovingTile::setMovingTileData(const MovingTileData& data) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, data.length * TILE_SIZE_F, 40.f));
	float phi = degToRad(static_cast<float>(data.initialDirection - 90));

	m_currentVelocity.x = std::round(data.speed * std::cos(phi)); 
	m_currentVelocity.y = std::round(data.speed * std::sin(phi));

	m_distanceTime = data.speed == 0 ? sf::Time::Zero : sf::seconds(static_cast<float>(data.distance) / static_cast<float>(data.speed));
	m_timeUntilTurn = m_distanceTime;

	setFrozen(data.isFrozen);
}

void MovingTile::init() {
	m_isAlwaysUpdate = true;
	m_isCollidable = true;
	m_isStrictlyCollidable = true;
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
	if (!m_isFrozen) {
		updateTime(m_timeUntilTurn, frameTime);
		if (m_timeUntilTurn == sf::Time::Zero) {
			m_timeUntilTurn = m_distanceTime;
			m_currentVelocity = -m_currentVelocity;
			m_relativeVelocity = m_currentVelocity;
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

const sf::Vector2f& MovingTile::getRelativeVelocity() const {
	return m_relativeVelocity;
}

GameObjectType MovingTile::getConfiguredType() const {
	return GameObjectType::_MovableTile;
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
	m_relativeVelocity = m_isFrozen ? sf::Vector2f() : m_currentVelocity;
	setPosition(getPosition());
}