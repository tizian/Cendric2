#include "stdafx.h"

MainCharacter::MainCharacter(Level* level) 
{
	MovableGameObject::MovableGameObject();
	m_level = level;
	load();
	setPosition(m_level->getStartPos());
}
MainCharacter::~MainCharacter() 
{
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
}

void MainCharacter::update(sf::Time& frameTime)
{
	handleInput();
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	updateAnimation();
}

void MainCharacter::checkCollisions(sf::Vector2f nextPosition)
{	
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // cendric is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	// check for collision on y axis
	bool collidesY = m_level->collidesY(nextBoundingBoxY);
	if (!isMovingDown && collidesY) 
	{
		setAccelerationY(0.0);
		setVelocityY(0.0f);
	}
	else if (isMovingDown && collidesY)
	{
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		// set cendric down.
		setPositionY(m_level->getGround(nextBoundingBoxY));
		m_isGrounded = true;
	}

	if (abs(getVelocity().y) > 0.0f)
	{
		m_isGrounded = false;
	}
}

void MainCharacter::updateAnimation()
{
	// calculate new game state and set animation.
	GameObjectState newState = GameObjectState::Idle;

	if (abs(getVelocity().x) > 20.0f)
	{
		newState = GameObjectState::Walking;
	}
	if (!m_isGrounded)
	{
		newState = GameObjectState::Jumping;
	}	

	// only update animation if we need to
	if (m_state != newState || m_nextIsFacingRight != m_isFacingRight)
	{
		m_isFacingRight = m_nextIsFacingRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
}

void MainCharacter::handleInput()
{
	float newAccelerationX = 0;
	float newAccelerationY = GRAVITY_ACCELERATION;

	if (g_inputController->isKeyActive(Key::Left))
	{
		m_nextIsFacingRight = false;
		newAccelerationX -= WALK_ACCELERATION;
	}
	if (g_inputController->isKeyActive(Key::Right))
	{
		m_nextIsFacingRight = true;
		newAccelerationX += WALK_ACCELERATION;
	}
	if (g_inputController->isKeyActive(Key::Jump) && m_isGrounded)
	{
		newAccelerationY = JUMP_BOOST;
	}

	setAcceleration(Vector2f(newAccelerationX, newAccelerationY));
}

Vector2f MainCharacter::getCenter()
{
	return Vector2f(
		getBoundingBox()->left + (getBoundingBox()->width / 2), 
		getBoundingBox()->top + (getBoundingBox()->height / 2));
}

void MainCharacter::load()
{
	setBoundingBox(sf::IntRect(0, 0, 46, 100));
	setSpriteOffset(sf::Vector2f(-17, -20));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	walkingAnimation.addFrame(sf::IntRect(0, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(80, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(160, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(240, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(320, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(400, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(480, 0, 80, 120));
	walkingAnimation.addFrame(sf::IntRect(560, 0, 80, 120));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	idleAnimation.addFrame(sf::IntRect(640, 0, 80, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	jumpingAnimation.addFrame(sf::IntRect(720, 0, 80, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}
