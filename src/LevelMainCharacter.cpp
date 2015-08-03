#include "LevelMainCharacter.h"

LevelMainCharacter::LevelMainCharacter(Level* level) : LevelMovableGameObject(level)
{
}

LevelMainCharacter::~LevelMainCharacter()
{
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
	m_keyMap.clear();
}

void LevelMainCharacter::handleAttackInput()
{
	// update current spell
	for (auto const &it : m_keyMap) {
		if (g_inputController->isKeyActive(it.first))
		{
			m_spellManager->setCurrentSpell(it.second);
			m_spellManager->executeCurrentSpell(g_inputController->getMousePosition());
		}
	}

	// handle attack input
	if (g_inputController->isMouseJustPressedLeft())
	{
		m_spellManager->executeCurrentSpell(g_inputController->getMousePosition());
	}
}

void LevelMainCharacter::handleMovementInput()
{
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
}

void LevelMainCharacter::setCharacterCore(CharacterCore* core)
{
	m_core = core;
	m_attributes = core->getTotalAttributes();
	setPosition(core->getData().currentLevelPosition);

	// these values should come from the staff
	m_keyMap.insert(
	{
		{ Key::Chop, SpellID::Chop },
		{ Key::FirstSpell, SpellID::IceBall },
		{ Key::SecondSpell, SpellID::FireBall },
		{ Key::ThirdSpell, SpellID::DivineShield },
		{ Key::FourthSpell, SpellID::Aureola },
		{ Key::FifthSpell, SpellID::VOID },
	});

	// TODO these values should be come from the staff
	SpellBean chopSpell = DEFAULT_CHOP;
	SpellBean iceSpell = DEFAULT_ICEBALL;
	iceSpell.inputKey = Key::FirstSpell;
	SpellBean fireSpell = DEFAULT_FIREBALL;
	fireSpell.inputKey = Key::SecondSpell;
	SpellBean divineShieldSpell = DEFAULT_DIVINESHIELD;
	divineShieldSpell.inputKey = Key::ThirdSpell;
	SpellBean aureolaSpell = DEFAULT_AUREOLA;
	aureolaSpell.inputKey = Key::FourthSpell;

	// TODO these modifiers should come from a staff, too
	SpellModifier level2DamageModifier;
	level2DamageModifier.level = 2;
	level2DamageModifier.type = SpellModifierType::Damage;
	SpellModifier level1CountModifier;
	level1CountModifier.level = 1;
	level1CountModifier.type = SpellModifierType::Count;
	SpellModifier level2ReflectModifier;
	level2ReflectModifier.level = 2;
	level2ReflectModifier.type = SpellModifierType::Reflect;

	std::vector<SpellModifier> fireModifiers;
	fireModifiers.push_back(level2DamageModifier);
	fireModifiers.push_back(level1CountModifier);
	fireModifiers.push_back(level2ReflectModifier);

	std::vector<SpellModifier> aureolaModifiers;
	aureolaModifiers.push_back(level2DamageModifier);
	aureolaModifiers.push_back(level1CountModifier);

	m_spellManager = new SpellManager(this);
	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(iceSpell);
	m_spellManager->addSpell(fireSpell, fireModifiers);
	m_spellManager->addSpell(divineShieldSpell);
	m_spellManager->addSpell(aureolaSpell, aureolaModifiers);
}

void LevelMainCharacter::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 100.f));
	setSpriteOffset(sf::Vector2f(-25.f, -20.f));

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
	fightingAnimation.addFrame(sf::IntRect(1040, 0, 80, 120));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	deadAnimation.addFrame(sf::IntRect(1120, 0, 80, 120));

	addAnimation(GameObjectState::Dead, deadAnimation);

	setFrameTime(sf::seconds(0.07f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);

	setDebugBoundingBox(sf::Color::White);
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

sf::Time LevelMainCharacter::getConfiguredFightAnimationTime() const
{
	return sf::milliseconds(4 * 70);
}

GameObjectType LevelMainCharacter::getConfiguredType() const
{
	return GameObjectType::_MainCharacter;
}

void LevelMainCharacter::lootItem(const std::string& item, int quantity) const
{
	std::map<std::string, int>* coreItems = m_core->getItems();
	auto it = coreItems->find(item);

	if (it != coreItems->end())
	{
		coreItems->at(item) = coreItems->at(item) + quantity;
	}
	else
	{
		coreItems->insert({item, quantity});
	}
}

void LevelMainCharacter::lootItems(std::map<std::string, int>& items) const
{
	for (auto it : items)
	{
		lootItem(it.first, it.second);
	}
}

void LevelMainCharacter::addGold(int gold) const
{
	m_core->addGold(gold);
}
