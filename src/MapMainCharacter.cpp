#include "MapMainCharacter.h"

MapMainCharacter::MapMainCharacter(Map* map)
{
	m_map = map;
	load();
	setPosition(m_map->getStartPos());
}

MapMainCharacter::~MapMainCharacter()
{
	g_resourceManager->deleteResource(ResourceID::Texture_mapMainChar);
}

void MapMainCharacter::update(sf::Time& frameTime)
{
	handleInput();
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	updateAnimation();
}

void MapMainCharacter::checkCollisions(sf::Vector2f nextPosition)
{
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingY = nextPosition.y != getBoundingBox()->top; 
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_map->collidesX(nextBoundingBoxX))
	{
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	// check for collision on y axis
	if (isMovingY && m_map->collidesY(nextBoundingBoxY))
	{
		setAccelerationY(0.0);
		setVelocityY(0.0f);
	}
}

void MapMainCharacter::updateAnimation()
{
	// calculate new game state and set animation.
	GameObjectState newState;
	// check if char walks up
	if (m_isFacingUp && (abs(getVelocity().x) < abs(getVelocity().y)))
	{
		if (getVelocity().y < -20.f) 
		{
			newState = GameObjectState::Walking_up;
		}
		else
		{
			newState = GameObjectState::Idle_up;
		}
	}
	else if (!m_isFacingUp && (abs(getVelocity().x) < abs(getVelocity().y)))
	{
		if (getVelocity().y > 20.f)
		{
			newState = GameObjectState::Walking_down;
		}
		else
		{
			newState = GameObjectState::Idle_down;
		}
	}
	else if (!m_isFacingRight && (abs(getVelocity().x) > abs(getVelocity().y)))
	{
		if (getVelocity().x < -20.f)
		{
			newState = GameObjectState::Walking_left;
		}
		else
		{
			newState = GameObjectState::Idle_left;
		}
	}
	else
	{
		if (getVelocity().x > 20.f)
		{
			newState = GameObjectState::Walking_right;
		}
		else
		{
			newState = GameObjectState::Idle_right;
		}
	}

	// keep state if vel is 0
	if (getVelocity().x == 0.f && getVelocity().y == 0.f)
	{
		return;
	}
	
	// only update animation if we need to
	if (m_state != newState)
	{
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), false);
	}
}

void MapMainCharacter::handleInput()
{
	float newAccelerationX = 0.f;
	float newAccelerationY = 0.f;

	if (g_inputController->isKeyActive(Key::Left))
	{
		newAccelerationX -= WALK_ACCELERATION;
		m_isFacingRight = false;
	}
	if (g_inputController->isKeyActive(Key::Right))
	{
		newAccelerationX += WALK_ACCELERATION;
		m_isFacingRight = true;
	}
	if (g_inputController->isKeyActive(Key::Up))
	{
		newAccelerationY -= WALK_ACCELERATION;
		m_isFacingUp = true;
	}
	if (g_inputController->isKeyActive(Key::Down))
	{
		newAccelerationY += WALK_ACCELERATION;
		m_isFacingUp = false;
	}

	setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void MapMainCharacter::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 48.f, 48.f));
	setSpriteOffset(sf::Vector2f(-1.f, -1.f));

	Animation walkingAnimationDown;
	walkingAnimationDown.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationDown.addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimationDown.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

	Animation walkingAnimationLeft;
	walkingAnimationLeft.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationLeft.addFrame(sf::IntRect(0, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(50, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(100, 50, 50, 50));
	walkingAnimationLeft.addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

	Animation walkingAnimationRight;
	walkingAnimationRight.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapMainChar));
	walkingAnimationRight.addFrame(sf::IntRect(0, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(50, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(100, 100, 50, 50));
	walkingAnimationRight.addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

	Animation walkingAnimationUp;
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

	setFrameTime(sf::seconds(0.15f));

	// initial values
	m_state = GameObjectState::Idle_right;
	m_isFacingRight = true;
	m_isFacingUp = false;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void MapMainCharacter::calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel)
{
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - DAMPING_PER_S, frameTime.asSeconds());
	nextVel.y = (getVelocity().y + getAcceleration().y * frameTime.asSeconds()) * pow(1 - DAMPING_PER_S, frameTime.asSeconds());
}

const float MapMainCharacter::getConfiguredMaxVelocityY()
{
	return 200.0f;
}

const float MapMainCharacter::getConfiguredMaxVelocityX()
{
	return 200.0f;
}