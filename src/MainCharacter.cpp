#include "stdafx.h"

MainCharacter::MainCharacter(Level* level) 
{
	MovableGameObject::MovableGameObject();
	m_level = level;
	load();
	setPosition(m_level->getStartPos());
}
MainCharacter::~MainCharacter() {}

void MainCharacter::update(sf::Time& frameTime)
{
	handleInput();
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
}

void MainCharacter::checkCollisions(sf::Vector2f nextPosition)
{

}

void MainCharacter::handleInput()
{

	GameObjectState newState = m_state;
	bool newDirectionRight = m_isFacingRight;
	bool anyKeyPressed = false;
	if (g_inputController->isKeyActive(Key::Left))
	{
		newState = GameObjectState::Walking;
		newDirectionRight = false;
		anyKeyPressed = true;
	}
	if (g_inputController->isKeyActive(Key::Right))
	{
		newState = GameObjectState::Walking;
		newDirectionRight = true;
		anyKeyPressed = true;
	}
	if (g_inputController->isKeyActive(Key::Jump))
	{
		newState = GameObjectState::Jumping;
		anyKeyPressed = true;
	}
	if (!anyKeyPressed)
	{
		newState = GameObjectState::Idle;
	}

	// only update animation if we need to
	if (m_state != newState || newDirectionRight != m_isFacingRight) 
	{
		m_isFacingRight = newDirectionRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
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

	setFrameTime(sf::seconds(0.1));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}
