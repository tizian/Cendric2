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

	// these values should be modified by a staff
	SpellBean fireSpell = DEFAULT_FIRE;
	SpellBean chopSpell = DEFAULT_CHOP;
	SpellBean forcefieldSpell = DEFAULT_FORCEFIELD;
	SpellBean iceSpell = DEFAULT_ICE;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(iceSpell);
	m_spellManager->addSpell(forcefieldSpell);
	m_spellManager->addSpell(fireSpell);

	// TODO [tiz] this is part of the interface. refactor / move. 
	m_hpBar.setFillColor(sf::Color::Red);
	m_hpBar.setSize(sf::Vector2f(0, BAR_HEIGHT));
	m_hpBar.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));
	m_hpBarOutline.setOutlineColor(sf::Color(150, 0, 0, 255));
	m_hpBarOutline.setOutlineThickness(BAR_OUTLINE_THICKNESS);
	m_hpBarOutline.setFillColor(sf::Color::Transparent);
	m_hpBarOutline.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));
	m_hpBarOutline.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
	m_manaBar.setFillColor(sf::Color::Blue);
	m_manaBar.setSize(sf::Vector2f(0, BAR_HEIGHT));
	m_manaBar.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + 3 * BAR_OUTLINE_THICKNESS + BAR_HEIGHT));
	m_manaBarOutline.setOutlineColor(sf::Color(0, 0, 150, 255));
	m_manaBarOutline.setOutlineThickness(BAR_OUTLINE_THICKNESS);
	m_manaBarOutline.setFillColor(sf::Color::Transparent);
	m_manaBarOutline.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + 3 * BAR_OUTLINE_THICKNESS + BAR_HEIGHT));
	m_manaBarOutline.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
}

LevelMainCharacter::~LevelMainCharacter()
{
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
	m_keyMap.clear();
	delete m_spellManager;
}

void LevelMainCharacter::updateInterfaceBars()
{
	m_hpBar.setSize(sf::Vector2f(BAR_WIDTH * (static_cast<float>(m_attributes->currentHealthPoints) / m_attributes->maxHealthPoints), BAR_HEIGHT));
	m_manaBar.setSize(sf::Vector2f(BAR_WIDTH * (static_cast<float>(m_attributes->currentManaPoints) / m_attributes->maxManaPoints), BAR_HEIGHT));
}

void LevelMainCharacter::onHit(Spell* spell)
{
	if (m_state == GameObjectState::Dead)
	{
		return;
	}
	// check for owner
	if (spell->getOwner() == this)
	{
		return;
	}
	int damage = 0;
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Ice:
		damage = spell->getDamage() - m_attributes->resistanceIce;
		spell->setDisposed();
		break;
	case SpellID::Fire:
		damage = spell->getDamage() - m_attributes->resistanceFire;
		spell->setDisposed();
		break;
	case SpellID::Chop:
		damage = spell->getDamage() - m_attributes->resistancePhysical;
		spell->setDisposed();
		break;
	default:
		break;
	}
	m_attributes->currentHealthPoints = m_attributes->currentHealthPoints - std::max(damage, 0);
	if (m_attributes->currentHealthPoints < 0)
	{
		m_attributes->currentHealthPoints = 0;
		m_isDead = true;
	}
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
	m_attributes = &(core->getData().attributes);
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

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mainChar));
	deadAnimation.addFrame(sf::IntRect(1120, 0, 80, 120));

	addAnimation(GameObjectState::Dead, deadAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

void LevelMainCharacter::render(sf::RenderTarget &renderTarget)
{
	LevelMovableGameObject::render(renderTarget);

	sf::View oldView = renderTarget.getView();
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_hpBarOutline);
	renderTarget.draw(m_hpBar);
	renderTarget.draw(m_manaBarOutline);
	renderTarget.draw(m_manaBar);
	renderTarget.setView(oldView);
}

void LevelMainCharacter::update(const sf::Time& frameTime)
{
	LevelMovableGameObject::update(frameTime);
	updateInterfaceBars();
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
