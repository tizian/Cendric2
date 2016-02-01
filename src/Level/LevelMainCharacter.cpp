#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/MOBBehavior/AttackingBehaviors/UserAttackingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/UserMovingBehavior.h"

LevelMainCharacter::LevelMainCharacter(Level* level) : LevelMovableGameObject(level) {
	m_spellManager = new SpellManager(this);
	m_isQuickcast = g_resourceManager->getConfiguration().isQuickcast;
}

LevelMainCharacter::~LevelMainCharacter() {
	g_resourceManager->deleteResource(ResourceID::Texture_mainChar);
	m_spellKeyMap.clear();
	delete m_debugInfo;
}

void LevelMainCharacter::load() {
	loadAnimation();
	loadBehavior();
}

MovingBehavior* LevelMainCharacter::createMovingBehavior() {
	UserMovingBehavior* behavior = new UserMovingBehavior(this);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDampingGroundPerS(0.999f);
	behavior->setFightAnimationTime(sf::milliseconds(5 * 70));
	return behavior;
}

AttackingBehavior* LevelMainCharacter::createAttackingBehavior(bool asAlly) {
	UserAttackingBehavior* behavior = new UserAttackingBehavior(this);
	behavior->setAttackInput(std::bind(&LevelMainCharacter::handleAttackInput, this));
	return behavior;
}

void LevelMainCharacter::handleAttackInput() {
	if (isDead()) return;
	if (m_fearedTime > sf::Time::Zero || m_stunnedTime > sf::Time::Zero) return;
	if (g_inputController->isActionLocked()) return;
	// update current spell
	for (auto const &it : m_spellKeyMap) {
		if (g_inputController->isKeyJustPressed(it.first)) {
			if (m_isQuickcast) {
				m_spellManager->setCurrentSpell(it.second);
				m_spellManager->executeCurrentSpell(g_inputController->getMousePosition());
				if (m_invisibilityLevel > 0) {
					setInvisibilityLevel(0);
				}
			}
			else {
				m_spellManager->setAndExecuteSpell(it.second);
			}
			g_inputController->lockAction();
		}
	}

	// handle attack input
	if (g_inputController->isMouseJustPressedLeft()) {
		m_spellManager->executeCurrentSpell(g_inputController->getMousePosition());
		g_inputController->lockAction();
		if (m_invisibilityLevel > 0) {
			setInvisibilityLevel(0);
		}
	}
}

void LevelMainCharacter::loadWeapon() {
	// chop is always set.
	m_spellKeyMap.clear();
	m_spellKeyMap.insert({ Key::Chop, 0 });

	if (m_core == nullptr || m_core->getWeapon() == nullptr) {
		g_logger->logWarning("LevelMainCharacter::loadWeapon", "character core is not set or weapon not found.");
		m_spellManager->addSpell(SpellData::getSpellData(SpellID::Chop));
		return;
	}
	const Weapon* weapon = m_core->getWeapon();

	int spellNr = 0;
	m_spellKeyMap.insert({ Key::FirstSpell, weapon->getCurrentSpellForSlot(0) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::SecondSpell, weapon->getCurrentSpellForSlot(1) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::ThirdSpell, weapon->getCurrentSpellForSlot(2) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::FourthSpell, weapon->getCurrentSpellForSlot(3) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::FifthSpell, weapon->getCurrentSpellForSlot(4) == SpellID::VOID ? -1 : ++spellNr });

	// get spells and add to spell manager
	m_spellManager->clearSpells();

	// handle chop spell
	SpellData chop = SpellData::getSpellData(SpellID::Chop);
	chop.boundingBox = weapon->getWeaponChopRect();
	chop.cooldown = weapon->getWeaponCooldown();
	chop.damage = weapon->getWeaponChopDamage();
	chop.iconTextureRect = sf::IntRect(weapon->getIconTextureLocation().x, weapon->getIconTextureLocation().y, 50, 50);
	m_spellManager->addSpell(chop);

	// handle other spells
	for (int i = 0; i < 5; i++) {
		if (weapon->getCurrentSpellForSlot(i) == SpellID::VOID) continue;
		SpellData newBean = SpellData::getSpellData(weapon->getCurrentSpellForSlot(i));
		switch (i) {
		case 0:
			newBean.inputKey = Key::FirstSpell;
			break;
		case 1:
			newBean.inputKey = Key::SecondSpell;
			break;
		case 2:
			newBean.inputKey = Key::ThirdSpell;
			break;
		case 3:
			newBean.inputKey = Key::FourthSpell;
			break;
		case 4:
			newBean.inputKey = Key::FifthSpell;
			break;
		default:
			// unexpected
			return;
		}

		// add modifiers for this slot
		if (weapon->getCurrentModifiersForSlot(i) == nullptr) {
			m_spellManager->addSpell(newBean);
			continue;
		}
		std::vector<SpellModifier> spellModifiers;
		for (auto& mod : *(weapon->getCurrentModifiersForSlot(i))) {
			spellModifiers.push_back(mod);
		}
		m_spellManager->addSpell(newBean, spellModifiers);
	}
}

void LevelMainCharacter::setCharacterCore(CharacterCore* core) {
	m_core = core;
	m_attributes = core->getTotalAttributes();
	setPosition(core->getData().currentLevelPosition);
	loadWeapon();
}

void LevelMainCharacter::setInvisibilityLevel(int level) {
	if (level < 0 || level > 4) return;
	m_invisibilityLevel = level;
	if (m_invisibilityLevel == 0) {
		dynamic_cast<LevelScreen*>(m_screen)->removeTypedBuffs(SpellID::Invisibility);
		setSpriteColor(sf::Color::White, sf::milliseconds(1));
	}
	else {
		// sets the color for a "sufficiently long" time. Other actions will reset invisibility.
		setSpriteColor(sf::Color(255, 255, 255, 75), sf::seconds(1000));
	}
}

int LevelMainCharacter::getInvisibilityLevel() const {
	return m_invisibilityLevel;
}

void LevelMainCharacter::addDamageOverTime(DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	sf::IntRect textureLocation((static_cast<int>(data.damageType)-1) * 50, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDotBuffToInterface(textureLocation, data.duration, data);
	LevelMovableGameObject::addDamageOverTime(data);
}

void LevelMainCharacter::setFeared(const sf::Time& fearedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setFeared(fearedTime);
	DamageOverTimeData data;
	data.isFeared = true;
	sf::IntRect textureLocation(250, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDotBuffToInterface(textureLocation, fearedTime, data);
}

void LevelMainCharacter::setStunned(const sf::Time& stunnedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setStunned(stunnedTime);
	DamageOverTimeData data;
	data.isStunned = true;
	sf::IntRect textureLocation(300, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDotBuffToInterface(textureLocation, stunnedTime, data);
}

void LevelMainCharacter::addDamage(int damage, DamageType damageType) {
	// damage taken will remove invisibility
	setInvisibilityLevel(0);
	LevelMovableGameObject::addDamage(damage, damageType);
}

void LevelMainCharacter::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);
	if (m_debugInfo) {
		m_debugInfo->setString("x: " + std::to_string(getPosition().x) + " y: " + std::to_string(getPosition().y));
		m_debugInfo->setPosition(getPosition() + sf::Vector2f(0.f, -30.f));
	}
}

void LevelMainCharacter::renderAfterForeground(sf::RenderTarget& target) {
	LevelMovableGameObject::renderAfterForeground(target);
	if (m_debugInfo) {
		target.draw(*m_debugInfo);
	}
}

void LevelMainCharacter::setDebugBoundingBox(const sf::Color &debugColor) {
	if (!g_resourceManager->getConfiguration().isDebugRendering) return;
	LevelMovableGameObject::setDebugBoundingBox(debugColor);
	delete m_debugInfo;
	m_debugInfo = new BitmapText();
	m_debugInfo->setColor(sf::Color::Red);
}

void LevelMainCharacter::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-25.f, -30.f));

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

	Animation fightingAnimation(sf::milliseconds(70));
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

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	setDebugBoundingBox(sf::Color::White);
}

GameObjectType LevelMainCharacter::getConfiguredType() const {
	return GameObjectType::_LevelMainCharacter;
}

void LevelMainCharacter::lootItem(const std::string& item, int quantity) const {
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange(item, quantity);
	}
}

void LevelMainCharacter::removeItems(const std::string& item, int quantity) const {
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange(item, -quantity);
	}
}

void LevelMainCharacter::lootItems(std::map<std::string, int>& items) const {
	for (auto& it : items) {
		lootItem(it.first, it.second);
	}
}

void LevelMainCharacter::addGold(int gold) const {
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange("gold", gold);
	}
}

void LevelMainCharacter::removeGold(int gold) const {
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange("gold", -gold);
	}
}
