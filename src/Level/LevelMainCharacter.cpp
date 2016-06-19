#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/MOBBehavior/AttackingBehaviors/UserAttackingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/UserMovingBehavior.h"
#include "Level/DamageNumbers.h"
#include "GlobalResource.h"

LevelMainCharacter::LevelMainCharacter(Level* level) : LevelMovableGameObject(level) {
	m_spellManager = new SpellManager(this);
	m_isQuickcast = g_resourceManager->getConfiguration().isQuickcast;
}

LevelMainCharacter::~LevelMainCharacter() {
	m_spellKeyMap.clear();
	delete m_ps;
}

void LevelMainCharacter::load() {
	m_isAlwaysUpdate = true;
	loadResources();
	loadAnimation();
	loadBehavior();

	m_damageNumbers = new DamageNumbers(this->isAlly());

	m_bloodPS = new particles::MetaballParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB), WINDOW_WIDTH, WINDOW_HEIGHT);
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB)->setSmooth(true);
	m_bloodPS->color = sf::Color(106, 12, 15, 220);

	auto spawner = m_bloodPS->addSpawner<particles::BoxSpawner>();
	spawner->size = sf::Vector2f(0.5f * getBoundingBox()->width, 0.4f * getBoundingBox()->height);
	m_bloodSpawner = spawner;

	auto timeGen = m_bloodPS->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.01f;
	timeGen->maxTime = 0.2f;

	auto sizeGen = m_bloodPS->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 12.f;
	sizeGen->minEndSize = 5.f;
	sizeGen->maxEndSize = 12.f;

	m_bloodVelGen = m_bloodPS->addGenerator<particles::AngledVelocityGenerator>();

	m_bloodPS->addUpdater<particles::TimeUpdater>();
	m_bloodPS->addUpdater<particles::SizeUpdater>();
	auto euler = m_bloodPS->addUpdater<particles::EulerUpdater>();
	euler->globalAcceleration = sf::Vector2f(0.f, 300.f);
}

void LevelMainCharacter::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);

	if (m_isDead) {
		m_ps->update(frameTime);
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_ps->emitRate = 0;
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
		return;
	}

	// Remove target if right-clicked anywhere
	if (g_inputController->isMouseClickedRight()) {
		setTargetEnemy(nullptr);
	}

	// enemy no longer targeted if it moves out of view
	if (m_targetedEnemy && !m_targetedEnemy->isViewable()) {
		setTargetEnemy(nullptr);
	}
	if (m_lastHitEnemy && !m_lastHitEnemy->isViewable()) {
		setLastHitEnemy(nullptr);
	}

	MainCharacter::handleInteraction();
}

void LevelMainCharacter::render(sf::RenderTarget& target) {
	LevelMovableGameObject::render(target);
	if (m_isDead) m_ps->render(target);
}


MovingBehavior* LevelMainCharacter::createMovingBehavior(bool asAlly) {
	UserMovingBehavior* behavior = new UserMovingBehavior(this);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDampingGroundPerS(0.999f);
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

	sf::Vector2f target = m_targetedEnemy != nullptr ? 
		// Target lock
		m_targetedEnemy->getCenter() :
		g_inputController->getMousePosition();

	// update current spell
	for (const auto& it : m_spellKeyMap) {
		if (g_inputController->isKeyJustPressed(it.first)) {
			if (m_isQuickcast) {
				m_spellManager->setCurrentSpell(it.second);
				m_spellManager->executeCurrentSpell(target);
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
		m_spellManager->executeCurrentSpell(target);
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
	chop.spellOffset.x = chop.boundingBox.left;
	chop.spellOffset.y = chop.boundingBox.top;
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
	m_spellManager->setCurrentSpell(0);
}

void LevelMainCharacter::setPosition(const sf::Vector2f& pos) {
	LevelMovableGameObject::setPosition(pos);
	if (!m_isDead) return;
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2.f;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * (2.f/3.f);
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
		setSpriteColor(COLOR_WHITE, sf::milliseconds(1));
	}
	else {
		// sets the color for a "sufficiently long" time. Other actions will reset invisibility.
		setSpriteColor(sf::Color(255, 255, 255, 75), sf::seconds(1000));
	}
}

int LevelMainCharacter::getInvisibilityLevel() const {
	return m_invisibilityLevel;
}

void LevelMainCharacter::setDead() {
	if (m_isDead) return;
	LevelMovableGameObject::setDead();
}

void LevelMainCharacter::setQuickcast(bool quickcast) {
	m_isQuickcast = quickcast;
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

void LevelMainCharacter::addDamage(int damage, DamageType damageType, bool overTime) {
	// damage taken will remove invisibility
	setInvisibilityLevel(0);
	LevelMovableGameObject::addDamage(damage, damageType, overTime);
}

void LevelMainCharacter::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-25.f, -30.f));
	sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 80, 0, 80, 120));
	}
	
	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(640, 0, 80, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(720, 0, 80, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 14; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 80, 0, 80, 120));
	}
	// duplicate last frame because of level equipment
	fightingAnimation->addFrame(sf::IntRect(1040, 0, 80, 120));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	setDebugBoundingBox(COLOR_WHITE);
	loadParticleSystem();
}

GameObjectType LevelMainCharacter::getConfiguredType() const {
	return GameObjectType::_LevelMainCharacter;
}

std::string LevelMainCharacter::getSpritePath() const {
	return "res/assets/cendric/spritesheet_cendric_level.png";
}

std::string LevelMainCharacter::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
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
	if (gold <= 0) return;
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange("gold", gold);
	}
}

void LevelMainCharacter::removeGold(int gold) const {
	if (gold >= 0) return;
	if (LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen)) {
		levelScreen->notifyItemChange("gold", -gold);
	}
}

void LevelMainCharacter::setTargetEnemy(Enemy* enemy) {
	if (m_targetedEnemy) m_targetedEnemy->setTargeted(false);
	m_targetedEnemy = enemy;
	if (m_targetedEnemy) m_targetedEnemy->setTargeted(true);
}

Enemy* LevelMainCharacter::getCurrentTargetEnemy() const {
	return m_targetedEnemy;
}

void LevelMainCharacter::setLastHitEnemy(Enemy* enemy) {
	if (m_lastHitEnemy) m_lastHitEnemy->setLastHit(false);
	m_lastHitEnemy = enemy;
	if (m_lastHitEnemy) m_lastHitEnemy->setLastHit(true);
}

Enemy* LevelMainCharacter::getLastHitEnemy() const {
	return m_lastHitEnemy;
}

bool LevelMainCharacter::isAlly() const {
	return true;
}

void LevelMainCharacter::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(300, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 100.f;

	// Generators
	auto posSpawner = m_ps->addSpawner<particles::DiskSpawner>();
	posSpawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posSpawner->radius = 20.f;
	m_particleSpawner = posSpawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}
