#include "Map/MapMainCharacter.h"
#include "Map/DynamicTiles/DoorMapTile.h"
#include "Screens/MapScreen.h"

MapMainCharacter::MapMainCharacter(Map* map) : MapMovableGameObject(map) {
	load();
}

MapMainCharacter::~MapMainCharacter() {
}

void MapMainCharacter::setCharacterCore(CharacterCore* core) {
	m_core = core;
	setPosition(m_core->getData().currentMapPosition);
}

void MapMainCharacter::update(const sf::Time& frameTime) {
	handleInput();
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	updateAnimation(frameTime);
	MainCharacter::handleInteraction();
}

void MapMainCharacter::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);
	WorldCollisionQueryRecord rec;

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool isMovingRight = nextPosition.x > getBoundingBox()->left;
	bool isMovingDown = nextPosition.y > getBoundingBox()->top;

	// check for collision on x axis
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (isMovingX && m_map->collides(rec)) {
		setAccelerationX(0.f);
		setVelocityX(0.f);
		if (!rec.noSafePos) {
			setPositionX(rec.safeLeft);
			nextBoundingBoxY.left = rec.safeLeft;
		}
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}

	// check for collision on y axis
	rec.boundingBox = nextBoundingBoxY;
	rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
	if (isMovingY && m_map->collides(rec)) {
		setAccelerationY(0.f);
		setVelocityY(0.f);
		if (!rec.noSafePos) {
			setPositionY(rec.safeTop);
		}
	}
}

void MapMainCharacter::render(sf::RenderTarget& target) {
	MapMovableGameObject::render(target);
	dynamic_cast<MapScreen*>(m_screen)->renderEquipment(target);
}

void MapMainCharacter::handleInput() {
	float newAccelerationX = 0.f;
	float newAccelerationY = 0.f;

	if (g_inputController->isKeyActive(Key::Left)) {
		newAccelerationX -= WALK_ACCELERATION;
	}
	else if (g_inputController->isKeyActive(Key::Right)) {
		newAccelerationX += WALK_ACCELERATION;
	}
	else if (g_inputController->isKeyActive(Key::Up)) {
		newAccelerationY -= WALK_ACCELERATION;
	}
	else if (g_inputController->isKeyActive(Key::Down)) {
		newAccelerationY += WALK_ACCELERATION;
	}

	setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void MapMainCharacter::load() {
	g_resourceManager->loadTexture(getSpritePath(), ResourceType::Map);
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	m_isAlwaysUpdate = true;
	setBoundingBox(sf::FloatRect(0.f, 0.f, 18.f, 15.f));
	setSpriteOffset(sf::Vector2f(-16.f, -35.f));

	Animation* walkingAnimationDown = new Animation(sf::seconds(0.15f));
	walkingAnimationDown->setSpriteSheet(tex);
	walkingAnimationDown->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

	Animation* walkingAnimationLeft = new Animation(sf::seconds(0.15f));
	walkingAnimationLeft->setSpriteSheet(tex);
	walkingAnimationLeft->addFrame(sf::IntRect(0, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(100, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

	Animation* walkingAnimationRight = new Animation(sf::seconds(0.15f));
	walkingAnimationRight->setSpriteSheet(tex);
	walkingAnimationRight->addFrame(sf::IntRect(0, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(100, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

	Animation* walkingAnimationUp = new Animation(sf::seconds(0.15f));
	walkingAnimationUp->setSpriteSheet(tex);
	walkingAnimationUp->addFrame(sf::IntRect(0, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(100, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

	Animation* idleAnimationDown = new Animation();
	idleAnimationDown->setSpriteSheet(tex);
	idleAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle_down, idleAnimationDown);

	Animation* idleAnimationLeft = new Animation();
	idleAnimationLeft->setSpriteSheet(tex);
	idleAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

	Animation* idleAnimationRight = new Animation();
	idleAnimationRight->setSpriteSheet(tex);
	idleAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Idle_right, idleAnimationRight);

	Animation* idleAnimationUp = new Animation();
	idleAnimationUp->setSpriteSheet(tex);
	idleAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Idle_up, idleAnimationUp);

	setDebugBoundingBox(COLOR_WHITE);

	// initial values
	m_state = GameObjectState::Idle_down;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void MapMainCharacter::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (getAcceleration().x == 0.f) {
		nextVel.x = 0.f;
	}
	else {
		nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds());
	}

	if (getAcceleration().y == 0.f) {
		nextVel.y = 0.f;
	}
	else {
		nextVel.y = (getVelocity().y + getAcceleration().y * frameTime.asSeconds());
	}
}

float MapMainCharacter::getConfiguredMaxVelocityYUp() const {
	return 200.f;
}

float MapMainCharacter::getConfiguredMaxVelocityYDown() const {
	return 200.f;
}

float MapMainCharacter::getConfiguredMaxVelocityX() const {
	return 200.f;
}

GameObjectType MapMainCharacter::getConfiguredType() const {
	return GameObjectType::_MapMovableGameObject;
}

std::string MapMainCharacter::getSpritePath() const {
	return "res/assets/cendric/spritesheet_cendric_map.png";
}