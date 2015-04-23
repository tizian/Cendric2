#include "MainCharacter.h"

MainCharacter::MainCharacter(Level* level) 
{
	m_level = level;
	load();
	setPosition(m_level->getStartPos());
	m_spellManager = new SpellManager();
}

MainCharacter::~MainCharacter() 
{
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
	delete m_spellManager;
}

void MainCharacter::update(sf::Time& frameTime)
{
	handleInput();
	m_spellManager->update(frameTime);
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	m_fightAnimationTime = (m_fightAnimationTime - frameTime) >= sf::Time::Zero ? m_fightAnimationTime - frameTime : sf::Time::Zero;
	updateAnimation();
}

void MainCharacter::render(sf::RenderTarget& renderTarget) const
{
	MovableGameObject::render(renderTarget);
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
	if (m_fightAnimationTime > sf::Time::Zero) 
	{
		newState = GameObjectState::Fighting;
	} 
	else if (!m_isGrounded)
	{
		newState = GameObjectState::Jumping;
	}
	else if (abs(getVelocity().x) > 20.0f)
	{
		newState = GameObjectState::Walking;
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
	// movement input
	float newAccelerationX = 0;

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
		setVelocityY(-getConfiguredMaxVelocityY()); // first jump vel will always be max y vel. 
	}

	setAcceleration(sf::Vector2f(newAccelerationX, GRAVITY_ACCELERATION));

	// handle attack input
	if (g_inputController->isMouseJustPressedLeft()) 
	{
		Spell* spell = m_spellManager->getSpell();
		if (spell != nullptr) 
		{
			spell->loadSpell(getLevel(), this);
			if (spell->getConfiguredTriggerFightAnimation()) {
				m_fightAnimationTime = sf::milliseconds(4 * 80); // duration of fight animation
			}
			m_screen->addObject(GameObjectType::_Spell, spell);
		}
	}
}

void MainCharacter::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 46.f, 100.f));
	setSpriteOffset(sf::Vector2f(-17.f, -20.f));

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

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	fightingAnimation.addFrame(sf::IntRect(800, 0, 80, 120));
	fightingAnimation.addFrame(sf::IntRect(880, 0, 80, 120));
	fightingAnimation.addFrame(sf::IntRect(960, 0, 80, 120));
	fightingAnimation.addFrame(sf::IntRect(1040, 0, 80, 120));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

void MainCharacter::calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel)
{
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? DAMPING_GROUND_PER_S : DAMPING_AIR_PER_S;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

const float MainCharacter::getConfiguredMaxVelocityY() 
{
	return 600.0f;
}

const float MainCharacter::getConfiguredMaxVelocityX()
{
	return 200.0f;
}

Level* MainCharacter::getLevel()
{
	return m_level;
}

bool MainCharacter::getIsFacingRight()
{
	return m_isFacingRight;
}

GameObjectState MainCharacter::getState()
{
	return m_state;
}

GameObjectType MainCharacter::getConfiguredType() const
{
	return GameObjectType::_MainCharacter;
}
