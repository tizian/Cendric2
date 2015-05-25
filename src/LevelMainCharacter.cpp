#include "LevelMainCharacter.h"

LevelMainCharacter::LevelMainCharacter(Level* level) : LevelMovableGameObject(level)
{
	load();
	setPosition(m_level->getStartPos());
	m_keyMap.insert(
	{
		{ Key::SpellChop, SpellID::Chop },
		{ Key::SpellIce, SpellID::Ice },
		{ Key::SpellFire, SpellID::Fire },
		{ Key::SpellForcefield, SpellID::Forcefield }
	});

	// TODO these values should come from a staff. they are hardcoded for now.
	SpellBean fireSpell;
	fireSpell.id = SpellID::Fire;
	fireSpell.maxActiveTime = sf::milliseconds(5000);
	fireSpell.cooldown = sf::milliseconds(1000);
	fireSpell.damage = 10;
	fireSpell.reflectCount = 0;
	fireSpell.startVelocity = 300.f;

	SpellBean chopSpell;
	chopSpell.id = SpellID::Chop;
	chopSpell.maxActiveTime = sf::milliseconds(320);
	chopSpell.cooldown = sf::milliseconds(400);
	chopSpell.damage = 2;
	chopSpell.reflectCount = 0;
	chopSpell.startVelocity = 0.f;
	chopSpell.boundingBox = sf::FloatRect(0, 0, 40, 60);

	SpellBean forcefieldSpell;
	forcefieldSpell.id = SpellID::Forcefield;
	forcefieldSpell.maxActiveTime = sf::milliseconds(3000);
	forcefieldSpell.cooldown = sf::milliseconds(10000);
	forcefieldSpell.damage = 1;
	forcefieldSpell.reflectCount = 0;
	forcefieldSpell.startVelocity = 0.f;

	SpellBean iceSpell;
	iceSpell.id = SpellID::Ice;
	iceSpell.maxActiveTime = sf::milliseconds(5000);
	iceSpell.cooldown = sf::milliseconds(1000);
	iceSpell.damage = 6;
	iceSpell.reflectCount = 0;
	iceSpell.startVelocity = 400.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(iceSpell);
	m_spellManager->addSpell(forcefieldSpell);
	m_spellManager->addSpell(fireSpell);
}

LevelMainCharacter::~LevelMainCharacter()
{
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
	m_keyMap.clear();
	delete m_spellManager;
}

void LevelMainCharacter::checkCollisions(const sf::Vector2f& nextPosition)
{	
	LevelMovableGameObject::checkCollisions(nextPosition);
	// TODO check positions with spells
}

void LevelMainCharacter::handleInput()
{
	// movement input
	float newAccelerationX = 0;

	if (g_inputController->isKeyActive(Key::Left))
	{
		m_nextIsFacingRight = false;
		newAccelerationX -= getConfiguredWalkAcceleration();
	}
	if (g_inputController->isKeyActive(Key::Right))
	{
		m_nextIsFacingRight = true;
		newAccelerationX += getConfiguredWalkAcceleration();
	}
	if (g_inputController->isKeyActive(Key::Jump) && m_isGrounded)
	{
		setVelocityY(-getConfiguredMaxVelocityY()); // first jump vel will always be max y vel. 
	}

	setAcceleration(sf::Vector2f(newAccelerationX, getConfiguredGravityAcceleration()));

	// update current spell
	for (auto const &it : m_keyMap) {
		if (g_inputController->isKeyActive(it.first))
		{
			m_spellManager->setCurrentSpell(it.second);
		}
	}

	// handle attack input
	if (g_inputController->isMouseJustPressedLeft()) 
	{
		Spell* spell = m_spellManager->getSpell();
		if (spell != nullptr) 
		{
			spell->loadSpell(getLevel(), this, g_inputController->getMousePosition());
			if (spell->getConfiguredTriggerFightAnimation()) {
				m_fightAnimationTime = sf::milliseconds(4 * 80); // duration of fight animation
			}
			m_screen->addObject(GameObjectType::_Spell, spell);
		}
	}
}

void LevelMainCharacter::setCharacterCore(CharacterCore* core)
{
	m_core = core;
}

void LevelMainCharacter::load()
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

float LevelMainCharacter::getConfiguredMaxVelocityY() const
{
	return 600.0f;
}

float LevelMainCharacter::getConfiguredMaxVelocityX() const
{
	return 200.0f;
}

float LevelMainCharacter::getConfiguredDampingGroundPersS() const
{
	return 0.999f;
}

GameObjectType LevelMainCharacter::getConfiguredType() const
{
	return GameObjectType::_MainCharacter;
}

sf::Color LevelMainCharacter::getConfiguredDebugColor() const
{
	return sf::Color::White;
}
