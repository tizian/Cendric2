#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/MOBBehavior/AttackingBehaviors/UserAttackingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/UserMovingBehavior.h"
#include "Level/Enemies/WardenEnemy.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "ScreenOverlays/TextureScreenOverlay.h"
#include "Level/DamageNumbers.h"
#include "GlobalResource.h"
#include "World/Item.h"
#include "Level/LevelEquipment.h"
#include "Level/LevelMainCharacterLoader.h"
#include "GUI/GamepadAimCursor.h"

LevelMainCharacter::LevelMainCharacter(Level* level) : LevelMovableGameObject(level) {
	m_spellManager = new SpellManager(this);
	m_targetManager = new TargetManager();
	
	m_isQuickcast = g_resourceManager->getConfiguration().isQuickcast;
	m_isAlwaysUpdate = true;
}

LevelMainCharacter::~LevelMainCharacter() {
	m_spellKeyMap.clear();
	delete m_targetManager;
}

void LevelMainCharacter::load() {
	m_targetManager->setMainCharacter(this);
	
	loadResources();
	loadAnimation();
	loadBehavior();

	m_damageNumbers = new DamageNumbers(this->isAlly());
	m_gamepadAimCursor = new GamepadAimCursor(this);

	setGodmode(g_resourceManager->getConfiguration().isGodmode);
}

void LevelMainCharacter::updateDamagedOverlay() {
	if (!m_damagedScreenOverlay && !m_isDead && static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints < 0.2f) {
		m_damagedScreenOverlay = new TextureScreenOverlay(sf::seconds(0.1f), sf::seconds(0.1f));
		m_damagedScreenOverlay->setPermanent(true);
		dynamic_cast<TextureScreenOverlay*>(m_damagedScreenOverlay)->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_OVERLAY_DAMAGED));
		m_screen->addObject(m_damagedScreenOverlay);
	}
	else if (m_damagedScreenOverlay && (m_isDead || static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints >= 0.2f)) {
		m_damagedScreenOverlay->setPermanent(false);
		m_damagedScreenOverlay = nullptr;
	}
}

void LevelMainCharacter::updateAutoscroller() {
	if (m_isDead || !m_autoscroller) return;

	auto pos = getPosition();
	auto minPos = std::min(m_autoscroller->getCameraLeft(), m_level->getWorldRect().width - WINDOW_WIDTH);
	auto maxPos = m_autoscroller->getCameraLeft() + WINDOW_WIDTH - getBoundingBox()->width;
	if (pos.x < minPos) {
		setPositionX(minPos);
	}
	else if (pos.x > maxPos) {
		setPositionX(maxPos);
	}
}

void LevelMainCharacter::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);
	updateDamagedOverlay();
	updateAutoscroller();

	if (m_needsAnimationReset) {
		m_animatedSprite.gotoFirstFrame();
		m_needsAnimationReset = false;
	}

	// update the sprite color time
	if (m_equipmentColoredTime > sf::Time::Zero) {
		updateTime(m_equipmentColoredTime, frameTime);
		if (m_equipmentColoredTime == sf::Time::Zero) {
			setSpriteColor(COLOR_WHITE, sf::Time::Zero);
		}
	}

	if (m_isDead) {
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_deathPc->setEmitRate(0.f);
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
	}
	if (!isReady()) return;

	m_targetManager->update(frameTime);
	handleInteraction();
}

void LevelMainCharacter::onHit(Spell* spell) {
	LevelMovableGameObject::onHit(spell);
	if (!m_targetManager->getCurrentTargetEnemy()) {
		m_targetManager->setTargetEnemy(const_cast<Enemy*>(dynamic_cast<const Enemy*>(spell->getOwner())));
	}
}

MovingBehavior* LevelMainCharacter::createMovingBehavior(bool asAlly) {
	UserMovingBehavior* behavior = new UserMovingBehavior(this);
	behavior->setMaxVelocityYUp(800.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDampingGroundPerS(0.9999f);
	behavior->setDampingAirPerS(0.9f);
	return behavior;
}

AttackingBehavior* LevelMainCharacter::createAttackingBehavior(bool asAlly) {
	UserAttackingBehavior* behavior = new UserAttackingBehavior(this);
	behavior->setAttackInput(std::bind(&LevelMainCharacter::handleAttackInput, this));
	return behavior;
}

sf::Vector2f LevelMainCharacter::getSelectedTarget() {
	if (!g_inputController->isJoystickConnected()) {
		return g_inputController->getMousePosition();
	}

	return m_gamepadAimCursor->getCurrentPosition();
}

void LevelMainCharacter::onSpellSelected() {
	g_inputController->lockAction();
	notifyGamepadCursor();
}

void LevelMainCharacter::notifyGamepadCursor() {
	auto spell = m_spellManager->getSelectedSpell();
	auto behavior = dynamic_cast<UserMovingBehavior*>(m_movingBehavior);
	m_gamepadAimCursor->setVisible(spell && spell->needsTarget && (!behavior || !behavior->isClimbing()));
}

void LevelMainCharacter::handleAttackInput() {
	if (m_isInputLock || isClimbing()) return;
	if (m_fearedTime > sf::Time::Zero || m_stunnedTime > sf::Time::Zero) return;
	if (g_inputController->isActionLocked()) return;

	bool isAttacking = g_inputController->isAttacking();
	bool isEnemyTargeted = m_targetManager->getCurrentTargetEnemy() != nullptr;
	CursorSkin cursorSkin = isEnemyTargeted ? TargetInactive : TargetActive;

	if (isAttacking) {
		g_inputController->getCursor().setCursorSkin(TargetHighlight, sf::seconds(0.2f), cursorSkin);
	}
	else {
		g_inputController->getCursor().setCursorSkin(cursorSkin);
	}

	bool useAutotarget = isEnemyTargeted && g_resourceManager->getConfiguration().isAutotarget &&
		(m_gamepadAimCursor->isUseAutotarget() || (!isAttacking && !g_inputController->isJoystickConnected()));

	sf::Vector2f target;
	if (useAutotarget) {
		target = m_targetManager->getCurrentTargetEnemy()->getCenter();
		m_gamepadAimCursor->setRotation(target - getSpellPosition());
	}
	else {
		target = getSelectedTarget();
	}

	// update current spell
	for (const auto& it : m_spellKeyMap) {
		if (g_inputController->isKeyJustPressed(it.first)) {
			if (it.second < 0) continue;
			auto spell = m_spellManager->getSpell(it.second);
			if (spell && contains(m_level->getLockedMagic(), spell->spellType)) {
				continue;
			}
			if (m_isQuickcast) {
				m_spellManager->setCurrentSpell(it.second);
				m_core->setWeaponSpell(it.first);
				m_spellManager->executeCurrentSpell(target);
				checkInvisibilityLevel();
			}
			else {
				m_spellManager->setAndExecuteSpell(it.second);
				m_core->setWeaponSpell(it.first);
			}
			onSpellSelected();
			return;
		}
	}

	// handle previous / next spell
	if (!g_inputController->isActionLocked()) {
		if (g_inputController->isKeyJustPressed(Key::NextSpell)) {
			m_spellManager->setNextSpell();
			onSpellSelected();
		}
		else if (g_inputController->isKeyJustPressed(Key::PreviousSpell)) {
			m_spellManager->setPreviousSpell();
			onSpellSelected();
		}
	}

	// handle attack input
	if (isAttacking) {
		m_spellManager->executeCurrentSpell(target);
		g_inputController->lockAction();
		checkInvisibilityLevel();
	}
}

void LevelMainCharacter::updateHealthRegeneration(const sf::Time& frameTime) {
	int oldHealth = m_attributes.currentHealthPoints;
	LevelMovableGameObject::updateHealthRegeneration(frameTime);
	if (oldHealth != m_attributes.currentHealthPoints) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyCharacterInfoReload();
	}
}

void LevelMainCharacter::checkInvisibilityLevel() {
	if (m_invisibilityLevel > 0 && 
		m_spellManager->getSelectedSpell()->spellType != SpellType::Twilight) {
		setInvisibilityLevel(0);
	}
}

sf::Vector2f LevelMainCharacter::getSpellPosition() const {
	return sf::Vector2f(m_boundingBox.left + m_boundingBox.width * 0.5f, m_boundingBox.top);
}

void LevelMainCharacter::loadWeapon() {
	clearOwnSpells();
	m_spellManager->clearSpells();
	// chop is always set.
	m_spellKeyMap.clear();
	m_spellKeyMap.insert({ Key::Chop, 0 });

	if (m_core == nullptr || m_core->getWeapon() == nullptr) {
		g_logger->logWarning("LevelMainCharacter::loadWeapon", "character core is not set or weapon not found.");
		m_spellManager->addSpell(SpellData::getSpellData(SpellID::Chop));
		m_spellManager->setCurrentSpell(0);
		onSpellSelected();
		return;
	}

	const Weapon* weapon = m_core->getWeapon();

	int spellNr = 0;
	m_spellKeyMap.insert({ Key::FirstSpell, weapon->getCurrentSpellForSlot(0) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::SecondSpell, weapon->getCurrentSpellForSlot(1) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::ThirdSpell, weapon->getCurrentSpellForSlot(2) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::FourthSpell, weapon->getCurrentSpellForSlot(3) == SpellID::VOID ? -1 : ++spellNr });
	m_spellKeyMap.insert({ Key::FifthSpell, weapon->getCurrentSpellForSlot(4) == SpellID::VOID ? -1 : ++spellNr });

	// handle chop spell
	auto const weaponBean = weapon->getBean<ItemWeaponBean>();
	SpellData chop = SpellData::getSpellData(SpellID::Chop);
	chop.creatorSoundPaths = {
		"res/sound/weapon/swing1.ogg",
		"res/sound/weapon/swing2.ogg",
		"res/sound/weapon/swing3.ogg"
	};
	chop.boundingBox = weaponBean->chop_rect;
	chop.spellOffset.x = chop.boundingBox.left;
	chop.spellOffset.y = chop.boundingBox.top;
	chop.cooldown = weaponBean->chop_cooldown;
	chop.damage = weaponBean->chop_damage;
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

	if (!m_spellManager->setCurrentSpell(getSpellFromKey(m_core->getData().weaponSpell))) {
		m_spellManager->setCurrentSpell(0);
		
	}
	else {
		auto currentSpell = m_spellManager->getSelectedSpell();
		if (currentSpell && contains(m_level->getLockedMagic(), currentSpell->spellType)) {
			m_spellManager->setCurrentSpell(0); // 0 is always chop and save
		}
	}
	
	// make sure a spell is always selected
	auto currentSpell = m_spellManager->getSelectedSpell();
	if (!currentSpell) {
		m_spellManager->setCurrentSpell(0);
	}

	if (!m_spellManager->getSpellMap().empty() && m_movingBehavior != nullptr) {
		const SpellData& spellData = m_spellManager->getSpellMap().at(0)->getSpellData();
		m_movingBehavior->setDefaultFightAnimation(spellData.fightingTime, spellData.fightAnimation);
	}

	onSpellSelected();
}

void LevelMainCharacter::setAutoscroller(AutoscrollerCamera* camera) {
	m_autoscroller = camera;
}

void LevelMainCharacter::setCharacterCore(CharacterCore* core) {
	m_core = core;
	setPosition(core->getData().currentLevelPosition);
	m_attributes = m_core->getTotalAttributes();
	loadWeapon();
}

void LevelMainCharacter::reloadEquipment() {
	// remove all equipment
	for (auto go : *m_screen->getObjects(_Equipment)) {
		go->setDisposed();
	}
	// reset our animation & load equipment new
	m_needsAnimationReset = true;
	LevelMainCharacterLoader::loadEquipment(m_screen);

	loadWeapon();
	reloadAttributes();
}

void LevelMainCharacter::reloadWeapon() {
	loadWeapon();
}

void LevelMainCharacter::clearOwnSpells() {
	for (auto go : *m_screen->getObjects(_Spell)) {
		auto spell = dynamic_cast<Spell*>(go);
		if (!spell || spell->isDisposed()) continue;
		
		if (spell->getOwner() == this) {
			spell->setDisposed();
		}
	}
}

void LevelMainCharacter::reloadAttributes() {
	// reloading attributes won't heal you, my dear.
	auto missingHealth = m_attributes.maxHealthPoints - m_attributes.currentHealthPoints;
	m_attributes = m_core->getTotalAttributes();
	m_attributes.currentHealthPoints = std::max(0, m_attributes.currentHealthPoints - missingHealth);
	// add food again
	if (isEating()) {
		m_attributes.addBean(m_foodAttributes.second);
	}
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
		setSpriteColor(sf::Color(255, 255, 255, 55 + level * 25), sf::seconds(1000));
	}

	for (auto go : *m_screen->getObjects(_Enemy)) {
		WardenEnemy* warden = dynamic_cast<WardenEnemy*>(go);
		if (warden) {
			warden->updateObservedRange();
		}
	}
}

int LevelMainCharacter::getInvisibilityLevel() const {
	return m_invisibilityLevel;
}

void LevelMainCharacter::setSpriteColor(const sf::Color& color, const sf::Time& time) {
	dynamic_cast<LevelScreen*>(m_screen)->setEquipmentColor(color);
	m_equipmentColoredTime = time;
}

void LevelMainCharacter::setDead() {
	if (m_isDead || m_isImmortal) return;
	LevelMovableGameObject::setDead();
	m_deathPc->setVisible(true);
	m_state = GameObjectState::Dead;
	setInputLock();
}

void LevelMainCharacter::setQuickcast(bool quickcast) {
	m_isQuickcast = quickcast;
}

void LevelMainCharacter::setGodmode(bool godmode) {
	m_isInvincible = godmode;
	m_isImmortal = godmode;
}

void LevelMainCharacter::addDamageOverTime(DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	sf::IntRect textureLocation((static_cast<int>(data.damageType) - 1) * 50, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDotBuffToInterface(textureLocation, data.duration, data);
	LevelMovableGameObject::addDamageOverTime(data);
}

void LevelMainCharacter::setFeared(const sf::Time& fearedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setFeared(fearedTime);
	DamageOverTimeData data;
	data.isFeared = true;
	sf::IntRect textureLocation(250, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDebuffBuffToInterface(textureLocation, fearedTime, data);
}

void LevelMainCharacter::setStunned(const sf::Time& stunnedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setStunned(stunnedTime);
	DamageOverTimeData data;
	data.isStunned = true;
	sf::IntRect textureLocation(300, 0, 50, 50);
	dynamic_cast<LevelScreen*>(m_screen)->addDebuffBuffToInterface(textureLocation, stunnedTime, data);
}

void LevelMainCharacter::addDamage(int damage, DamageType damageType, bool overTime, bool critical) {
	// damage taken will remove invisibility
	setInvisibilityLevel(0);
	LevelMovableGameObject::addDamage(damage, damageType, overTime, critical);
}

void LevelMainCharacter::loadAnimation() {
	int width = 80;
	int height = 120;
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-25.f, -30.f));
	sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, 0, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* climbing1Animation = new Animation();
	climbing1Animation->setSpriteSheet(tex);
	climbing1Animation->addFrame(sf::IntRect(14 * width, 0, width, height));
	climbing1Animation->setLooped(false);

	addAnimation(GameObjectState::Climbing_1, climbing1Animation);

	Animation* climbing2Animation = new Animation();
	climbing2Animation->setSpriteSheet(tex);
	climbing2Animation->addFrame(sf::IntRect(15 * width, 0, width, height));
	climbing2Animation->setLooped(false);

	addAnimation(GameObjectState::Climbing_2, climbing2Animation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 14; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	// duplicate last frame because of level equipment
	fightingAnimation->addFrame(sf::IntRect(13 * width, 0, width, height));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	setDebugBoundingBox(COLOR_WHITE);
	loadComponents();
}

TargetManager& LevelMainCharacter::getTargetManager() const {
	return *m_targetManager;
}

GameObjectType LevelMainCharacter::getConfiguredType() const {
	return _LevelMainCharacter;
}

std::string LevelMainCharacter::getSpritePath() const {
	return "res/texture/cendric/spritesheet_cendric_level.png";
}

std::string LevelMainCharacter::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
}

void LevelMainCharacter::setInputLock() {
	m_isInputLock = true;
	m_movingBehavior->setEnabled(false);
	m_animatedSprite.stop();
	m_animatedSprite.setLooped(false);

	auto gos = *m_screen->getObjects(_Equipment);
	for (auto go : gos) {
		LevelEquipment* le = dynamic_cast<LevelEquipment*>(go);
		if (!le) continue;
		le->lockAnimation(true);
	}
}

void LevelMainCharacter::setJumpLock() {
	dynamic_cast<UserMovingBehavior*>(m_movingBehavior)->setJumpLock();
}

void LevelMainCharacter::resetTarget() {
	m_targetManager->setTargetEnemy(nullptr);
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

bool LevelMainCharacter::isAlly() const {
	return true;
}

bool LevelMainCharacter::isReady() const {
	return LevelMovableGameObject::isReady() && !m_isInputLock;
}

bool LevelMainCharacter::isClimbing() const {
	return getState() == GameObjectState::Climbing_1 ||
		getState() == GameObjectState::Climbing_2;
}

void LevelMainCharacter::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;

	// Generators
	auto spawner = new particles::DiskSpawner();
	spawner->radius = 20.f;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;
	data.timeGen = timeGen;

	m_deathPc = new ParticleComponent(data, this);
	m_deathPc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.8f));
	m_deathPc->setVisible(false);
	addComponent(m_deathPc);
}

bool LevelMainCharacter::collides(const sf::Vector2f& nextPos) const {
	return LevelMovableGameObject::collides(nextPos) || m_level->collidesWithCamera(nextPos);
}

int LevelMainCharacter::getSpellFromKey(Key key) {
	switch (key) {
	case Key::Chop:
	default:
		return 0;
	case Key::FirstSpell:
		return 1;
	case Key::SecondSpell:
		return 2;
	case Key::ThirdSpell:
		return 3;
	case Key::FourthSpell:
		return 4;
	case Key::FifthSpell:
		return 5;
	}
}
