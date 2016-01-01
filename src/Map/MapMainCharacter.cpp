#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"

MapMainCharacter::MapMainCharacter(Map* map) : MapMovableGameObject(map) {
	load();
}

MapMainCharacter::~MapMainCharacter() {
	g_resourceManager->deleteResource(ResourceID::Texture_mapMainChar);
}

void MapMainCharacter::setCharacterCore(CharacterCore* core) {
	m_core = core;
	setPosition(m_core->getData().currentMapPosition);
}

void MapMainCharacter::update(const sf::Time& frameTime) {
	handleInput();
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	updateAnimation(frameTime);
}

void MapMainCharacter::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_map->collidesX(nextBoundingBoxX)) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}
	// check for collision on y axis
	if (isMovingY && m_map->collidesY(nextBoundingBoxY)) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
	}
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
	setBoundingBox(sf::FloatRect(0.f, 0.f, 10.f, 10.f));
	setSpriteOffset(sf::Vector2f(-20.f, -40.f));

	Animation walkingAnimationDown(sf::seconds(0.15f));
	walkingAnimationDown.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationDown.addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

	Animation walkingAnimationLeft(sf::seconds(0.15f));
	walkingAnimationLeft.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationLeft.addFrame(sf::IntRect(0, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(50, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(100, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

	Animation walkingAnimationRight(sf::seconds(0.15f));
	walkingAnimationRight.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationRight.addFrame(sf::IntRect(0, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(50, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(100, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

	Animation walkingAnimationUp(sf::seconds(0.15f));
	walkingAnimationUp.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationUp.addFrame(sf::IntRect(0, 150, 50, 50));
	walkingAnimationUp.addFrame(sf::IntRect(50, 150, 50, 50));
	walkingAnimationUp.addFrame(sf::IntRect(100, 150, 50, 50));
	walkingAnimationUp.addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

	Animation idleAnimationDown;
	idleAnimationDown.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	idleAnimationDown.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle_down, idleAnimationDown);

	Animation idleAnimationLeft;
	idleAnimationLeft.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	idleAnimationLeft.addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

	Animation idleAnimationRight;
	idleAnimationRight.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	idleAnimationRight.addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Idle_right, idleAnimationRight);

	Animation idleAnimationUp;
	idleAnimationUp.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	idleAnimationUp.addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Idle_up, idleAnimationUp);

	// initial values
	m_state = GameObjectState::Idle_down;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void MapMainCharacter::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (getAcceleration().x == 0.0f) {
		nextVel.x = 0;
	}
	else {
		nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds());
	}

	if (getAcceleration().y == 0.0f) {
		nextVel.y = 0;
	}
	else {
		nextVel.y = (getVelocity().y + getAcceleration().y * frameTime.asSeconds());
	}
}

float MapMainCharacter::getConfiguredMaxVelocityY() const {
	return 200.0f;
}

float MapMainCharacter::getConfiguredMaxVelocityX() const {
	return 200.0f;
}

GameObjectType MapMainCharacter::getConfiguredType() const {
	return GameObjectType::_MapMovableGameObject;
}