#include "Level/Enemies/VeliusBoss.h"
#include "Level/Enemies/VeliusBossClone.h"
#include "Level/Enemies/VeliusVictim.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/VeliusBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/DynamicTiles/MirrorTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Velius, VeliusBoss)

const sf::Color VeliusBoss::V_COLOR_ILLUSION = sf::Color(134, 63, 199);
const sf::Color VeliusBoss::V_COLOR_TWILIGHT = sf::Color(72, 80, 199);
const sf::Color VeliusBoss::V_COLOR_NECROMANCY = sf::Color(50, 199, 50);
const sf::Color VeliusBoss::V_COLOR_DIVINE = sf::Color(255, 255, 150);
const sf::Color VeliusBoss::V_COLOR_ELEMENTAL = sf::Color(244, 103, 50);

const sf::Time VeliusBoss::BLOCKING_TIME = sf::seconds(3.f);
const sf::Time VeliusBoss::BLOCKING_GRACE_TIME = sf::seconds(0.5f);

const sf::Time VeliusBoss::SHACKLE_DURATION = sf::seconds(7.f);
const sf::Time VeliusBoss::CALLING_TIME = sf::seconds(5.f);

const sf::Time VeliusBoss::CLONE_TIMEOUT = sf::seconds(10.f);
const std::vector<sf::Vector2f> VeliusBoss::CLONE_LOCATIONS = {
	sf::Vector2f(110.f, 285.f),
	sf::Vector2f(1210.f, 285.f),
	sf::Vector2f(1060.f, 535.f),
	sf::Vector2f(260.f, 535.f),
	sf::Vector2f(360.f, 385.f),
	sf::Vector2f(960.f, 385.f),
	sf::Vector2f(810.f, 285.f),
	sf::Vector2f(510.f, 285.f),
};

VeliusBoss::VeliusBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
	m_veliusLevel = 1;
	m_bossState = AttackIllusion;
	m_blockingBubble = nullptr;
}

VeliusBoss::~VeliusBoss() {
	delete m_ray;
}

void VeliusBoss::update(const sf::Time& frameTime) {
	Boss::update(frameTime);
	updateBossState(frameTime);
	if (m_ray) m_ray->update(frameTime);
}

void VeliusBoss::render(sf::RenderTarget& target) {
	Boss::render(target);
	if (m_ray) m_ray->render(target);
}

void VeliusBoss::loadAttributes() {
	m_attributes.setHealth(2500);
	m_attributes.resistancePhysical = -2000;
	m_attributes.resistanceIce = -1000;
	m_attributes.resistanceLight = -1000;
	m_attributes.resistanceFire = -1000;
	m_attributes.resistanceShadow = -1000;
	m_attributes.critical = 30;
	m_attributes.calculateAttributes();
}

void VeliusBoss::updateBossState(const sf::Time& frameTime) {
	switch (m_bossState)
	{
	case AttackIllusion:
		handleAttackPhase(frameTime, 2250, 2000);
		break;
	case ExtractTwilight:
		handleExtractPhase(frameTime, V_COLOR_TWILIGHT);
		break;
	case AttackTwilight:
		handleAttackPhase(frameTime, 1750, 1500);
		break;
	case ExtractNecromancy:
		handleExtractPhase(frameTime, V_COLOR_NECROMANCY);
		break;
	case AttackNecromancy:
		handleAttackPhase(frameTime, 1250, 1000);
		break;
	case ExtractDivine:
		handleExtractPhase(frameTime, V_COLOR_DIVINE);
		break;
	case AttackDivine:
		handleAttackPhase(frameTime, 750, 500);
		break;
	case ExtractElemental: 
		break;
	default:
		break;
	}
}

void VeliusBoss::startAttackPhase() {
	m_timeUntilClones = CLONE_TIMEOUT;
	m_isClonesDoneInPhase = false;
	m_isShackling = false;
}

void VeliusBoss::handleAttackPhase(const sf::Time& frameTime, int shackleThreshold, int extractThreshold) {
	if (m_isClones) return;

	updateBlocking(frameTime);
	updateClonesStart(frameTime, shackleThreshold);
	updateShackle(frameTime, extractThreshold);
}

void VeliusBoss::handleExtractPhase(const sf::Time& frameTime, const sf::Color& color) {
	if (m_isCalling) {
		updateTime(m_timeUntilCallingComplete, frameTime);
		if (m_timeUntilCallingComplete == sf::Time::Zero) {
			startExtraction(color);
		}
	}
	else {
		updateExtraction();
	}
}

void VeliusBoss::callToDie(const sf::Color& color) {
	m_level->setBackgroundLayerColor(color);
	m_isCalling = true;
	m_timeUntilCallingComplete = CALLING_TIME;
	setPosition(sf::Vector2f(800.f, 535.f));
	setReady();
	setFacingRight(false);
	setState(GameObjectState::Broken);
	setCurrentAnimation(getAnimation(m_state), true);

	const std::string calleeName = m_bossState == ExtractTwilight ? "Koray" :
		m_bossState == ExtractNecromancy ? "Robert" : "Inina";

	m_scriptedBehavior->say("Velius" + calleeName + "Call", 5);

	VeliusVictim* callee = nullptr;
	for (auto go : *m_screen->getObjects(_Enemy)) {
		callee = dynamic_cast<VeliusVictim*>(go);
		if (!callee) continue;
		if (callee->getEnemyName().compare(calleeName) == 0) {
			break;
		}
	}

	m_victim = callee;
	if (!callee) return;

	callee->callToDie();
}

void VeliusBoss::startExtraction(const sf::Color& color) {
	for (auto go : *m_screen->getObjects(GameObjectType::_DynamicTile)) {
		if (auto mirrorTile = dynamic_cast<MirrorTile*>(go)) {
			mirrorTile->reset();
		}
	}

	m_isCalling = false;
	m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
	m_ray->initRay(sf::Vector2f(675.f, 599.f), sf::Vector2f(1.f, -0.25f), color);
	setFacingRight(false);
	setState(GameObjectState::Casting3);
	setCurrentAnimation(getAnimation(m_state), true);
}

void VeliusBoss::startLastPhase() {
	m_level->setBackgroundLayerColor(V_COLOR_ELEMENTAL);
	setPosition(sf::Vector2f(800.f, 535.f));
	setReady();
	setFacingRight(true);
	setState(GameObjectState::Broken);
	setCurrentAnimation(getAnimation(m_state), true);

	m_scriptedBehavior->say("VeliusCendricCall", 5);
}

void VeliusBoss::updateExtraction() {
	bool victimDead = m_victim && m_victim->isDead();
	bool veliusHit = m_ray->intersectsBox(*getBoundingBox());

	if (!victimDead && !veliusHit) return;

	std::string veliusSpeech = "VeliusCendricDead";
	VeliusBossState nextState = Over;
	switch (m_bossState)
	{
	case ExtractTwilight:
		veliusSpeech = "VeliusKorayDead";
		nextState = AttackTwilight;
		break;
	case ExtractNecromancy:
		veliusSpeech = "VeliusRobertDead";
		nextState = AttackNecromancy;
		break;
	case ExtractDivine:
		veliusSpeech = "VeliusIninaDead";
		nextState = AttackDivine;
		break;
	default:
		break;
	}

	if (victimDead) {
		m_scriptedBehavior->say(veliusSpeech, 5);
		m_veliusLevel++;
	}
	else {
		setStunned(sf::seconds(2.f));
		m_victim->release();
	}

	delete m_ray;
	m_ray = nullptr;
	clearPuzzleBlocks();
	setState(GameObjectState::Idle);
	setBossState(nextState);
}

void VeliusBoss::setBossState(VeliusBossState state) {
	delete m_ray;
	m_ray = nullptr;
	m_bossState = state;
	switch (m_bossState)
	{
	case AttackIllusion:
		m_level->setBackgroundLayerColor(V_COLOR_ILLUSION);
		startAttackPhase();
		break;

	case ExtractDivine:
	{
		setupDivinePuzzle();
		callToDie(V_COLOR_DIVINE);
		break;
	}
	case ExtractNecromancy:
	{
		setupNecromancyPuzzle();
		callToDie(V_COLOR_NECROMANCY);
		break;
	}
	case ExtractTwilight:
	{
		setupTwilightPuzzle();
		callToDie(V_COLOR_TWILIGHT);
		break;
	}
	case AttackTwilight:
		startAttackPhase();
		break;
	case ExtractElemental:
		startLastPhase();
	default:
		break;
	}
}

void VeliusBoss::updateShackle(const sf::Time& frameTime, int healthThreshold) {
	if (!m_isShackling && m_attributes.currentHealthPoints < healthThreshold) {
		m_isShackling = true;
		setPosition(sf::Vector2f(650.f, 485.f));
		setReady();
		dynamic_cast<WalkingBehavior*>(m_movingBehavior)->stopAll();
		m_scriptedBehavior->say("VeliusEnough", 5);
		setBlocking();
		m_spellManager->setCurrentSpell(0);
		m_spellManager->executeCurrentSpell(m_mainChar, true);
		m_timeUntilShackleDone = sf::seconds(5.5f);
		return;
	}

	if (!m_isShackling) return;

	updateTime(m_timeUntilShackleDone, frameTime);
	if (m_timeUntilShackleDone == sf::Time::Zero) {
		m_isShackling = false;

		switch (m_bossState)
		{
		case AttackIllusion:
			setBossState(ExtractTwilight);
			break;
		case AttackTwilight:
			setBossState(ExtractNecromancy);
			break;
		case AttackNecromancy:
			setBossState(ExtractDivine);
			break;
		case AttackDivine:
			setBossState(ExtractElemental);
			break;
		default:
			break;
		}
	}
}

void VeliusBoss::clearClones() {
	for (auto go : *m_screen->getObjects(_Enemy)) {
		auto enemy = dynamic_cast<Enemy*>(go);
		if (!enemy) continue;
		if (enemy->getEnemyID() == EnemyID::VeliusClone) {
			enemy->setDead();
			enemy->setDisposed();
		}
	}
	m_isClones = false;
}

void VeliusBoss::setupClones(int phase) {
	clearBlocking();
	m_timeUntilClones = CLONE_TIMEOUT;
	m_isClonesDoneInPhase = true;
	m_isClones = true;

	int skinNr = phase;
	int noLocations = static_cast<int>(CLONE_LOCATIONS.size());

	// velius' position:
	sf::Vector2f veliusPos = CLONE_LOCATIONS[rand() % noLocations];
	setPosition(veliusPos);
	m_mainChar->resetTarget();

	// the clones' location
	for (auto& loc : CLONE_LOCATIONS) {
		if (loc.x == veliusPos.x && loc.y == veliusPos.y) continue;

		auto clone = dynamic_cast<LevelScreen*>(m_screen)->spawnEnemy(EnemyID::VeliusClone, loc, skinNr);
		clone->setFacingRight(rand() % 2 == 0);
	}
}

void VeliusBoss::updateClonesStart(const sf::Time& frameTime, int damageThreshold) {
	if (m_isShackling) return;
	updateTime(m_timeUntilClones, frameTime);
	if (m_timeUntilClones == sf::Time::Zero ||
		(!m_isClonesDoneInPhase && m_attributes.currentHealthPoints < damageThreshold)) {
		switch (m_bossState)
		{
		case AttackIllusion:
			setupClones(0);
			break;
		case AttackTwilight:
			setupClones(1);
			break;
		case AttackNecromancy:
			setupClones(2);
			break;
		case AttackDivine:
			setupClones(3);
			break;
		default:
			break;
		}
	}
}

void VeliusBoss::updateBlocking(const sf::Time& frameTime) {
	if (m_isIndefinitelyBlocking) return;

	if (m_isBlocking) {
		updateTime(m_blockingTime, frameTime);
		if (m_blockingTime == sf::Time::Zero) {
			clearBlocking();
		}
		return;
	}

	if (m_timeUntilBlocking > sf::Time::Zero) {
		updateTime(m_timeUntilBlocking, frameTime);
		if (m_timeUntilBlocking == sf::Time::Zero) {
			m_blockingGraceTime = BLOCKING_GRACE_TIME;
			m_blockingBubble->setEmitRate(400.f);
		}
		return;
	}

	updateTime(m_blockingGraceTime, frameTime);
	if (m_blockingGraceTime == sf::Time::Zero) {
		m_isBlocking = true;
		m_blockingTime = BLOCKING_TIME;
	}
}

void VeliusBoss::setBlocking() {
	m_isBlocking = true;
	m_isIndefinitelyBlocking = true;
	m_blockingBubble->setEmitRate(400.f);
}

void VeliusBoss::clearBlocking() {
	m_isBlocking = false;
	m_isIndefinitelyBlocking = false;
	m_blockingBubble->setEmitRate(0.f);
	m_timeUntilBlocking = sf::seconds(randomFloat(4.f, 8.f));
}

void VeliusBoss::loadSpells() {
	// shackle spell
	SpellData shackle;
	shackle.id = SpellID::Shackle;
	shackle.activeDuration = SHACKLE_DURATION;
	shackle.duration = SHACKLE_DURATION + sf::seconds(5.f);
	shackle.isBlocking = true;
	shackle.castingTime = sf::seconds(0.5f);
	shackle.castingAnimation = GameObjectState::Casting2;
	shackle.fightingTime = sf::seconds(5.f);
	shackle.fightAnimation = GameObjectState::Casting2;
	shackle.cooldown = sf::seconds(0.f);
	shackle.spritesheetPath = "res/texture/spells/spritesheet_spell_shackle.png";
	shackle.boundingBox = sf::FloatRect(0, 0, 10, 10);
	shackle.spellOffset = sf::Vector2f(0.f, -40.f);
	m_spellManager->addSpell(shackle);

	// illusion spell
	SpellData spell = SpellData::getSpellData(SpellID::FireBall);
	spell.skinNr = 6;
	spell.damage = 30;
	spell.count = 3;
	spell.damageType = DamageType::Shadow;
	spell.duration = sf::seconds(1.f);
	spell.cooldown = sf::seconds(5.f);
	spell.isBlocking = true;
	spell.fightingTime = sf::seconds(0.f);
	spell.castingTime = sf::seconds(0.45f);
	spell.castingAnimation = GameObjectState::Casting;
	spell.isDynamicTileEffect = false;
	spell.speed = 300;
	spell.spellOffset = sf::Vector2f(10.f, 0.f);

	m_spellManager->addSpell(spell);
	m_spellManager->setInitialCooldown(sf::seconds(2.f), SpellID::FireBall);

	spell.count = 1;
	m_spellManager->addSpell(spell);

	// twilight spell
	spell.skinNr = 7;
	spell.damageType = DamageType::Ice;
	spell.isStunning = true;
	spell.duration = sf::seconds(1.f);
	spell.count = 2;

	m_spellManager->addSpell(spell);

	spell.count = 1;
	m_spellManager->addSpell(spell);

	// necromancy spell
	SpellData necro = SpellData::getSpellData(SpellID::Leech);
	necro.damage = 30;
	necro.count = 3;
	necro.cooldown = sf::seconds(5.f);
	necro.isBlocking = true;
	necro.fightingTime = sf::seconds(0.f);
	necro.castingTime = sf::seconds(0.45f);
	necro.castingAnimation = GameObjectState::Casting;
	necro.speed = 300;
	necro.spellOffset = sf::Vector2f(10.f, 0.f);

	m_spellManager->addSpell(necro);
	m_spellManager->setInitialCooldown(sf::seconds(2.f), SpellID::Leech);

	necro.count = 1;
	m_spellManager->addSpell(necro);

	// divine spell
	SpellData divine = SpellData::getSpellData(SpellID::Aureola);
	divine.damage = 30;
	divine.count = 4;
	divine.cooldown = sf::seconds(5.f);
	divine.isBlocking = true;
	divine.fightingTime = sf::seconds(0.f);
	divine.castingTime = sf::seconds(0.45f);
	divine.castingAnimation = GameObjectState::Casting;
	divine.speed = 300;
	divine.spellOffset = sf::Vector2f(10.f, 0.f);

	m_spellManager->addSpell(divine);
	m_spellManager->setInitialCooldown(sf::seconds(2.f), SpellID::Aureola);

	divine.count = 2;
	m_spellManager->addSpell(divine);

	// initial boss state
	setBossState(AttackIllusion);
}

void VeliusBoss::handleAttackInput() {
	if (m_isShackling) return;

	switch (m_bossState)
	{
	case AttackIllusion:
		m_spellManager->setCurrentSpell(m_isClones ? 2 : 1);
		break;
	case AttackTwilight:
		m_spellManager->setCurrentSpell(m_isClones ? 4 : 3);
		break;
	case AttackNecromancy:
		m_spellManager->setCurrentSpell(m_isClones ? 6 : 5);
		break;
	case AttackDivine:
		m_spellManager->setCurrentSpell(m_isClones ? 8 : 7);
		break;

	case ExtractDivine:
	case ExtractNecromancy:
	case ExtractTwilight:
	case ExtractElemental:
		return;
	default:
		break;
	}
	m_spellManager->executeCurrentSpell(m_mainChar); // illusion attack
}

void VeliusBoss::onHit(Spell* spell) {
	if (m_isDead) return;

	if (m_isClones) {
		clearClones();
		setStunned(sf::seconds(1.f));
	}

	if (!m_isBlocking) {
		Enemy::onHit(spell);
		return;
	}

	if (!spell->isAttachedToMob()) {
		// we'll send you right back!
		spell->setOwner(this);
		spell->reflect();
	}
}

void VeliusBoss::setupTwilightPuzzle() {
	LevelTileProperties properties;

	auto positions = {
		sf::Vector2f(720.f, 350.f),
		sf::Vector2f(600.f, 175.f),
		sf::Vector2f(700.f, 175.f)
	};
		
	for (auto pos : positions) {
			LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::SwitchableOn, dynamic_cast<LevelScreen*>(m_screen));
			tile->init(properties);
			tile->setPosition(pos);
			tile->loadResources();
			tile->loadAnimation(7);
			tile->setPosition(pos);
			tile->setDebugBoundingBox(COLOR_NEUTRAL);
			m_screen->addObject(tile);
			m_puzzleBlocks.push_back(tile);
	}
}

void VeliusBoss::setupNecromancyPuzzle() {
	LevelTileProperties properties;

	auto positions = {
		sf::Vector2f(720.f, 350.f),
		sf::Vector2f(600.f, 175.f),
		sf::Vector2f(700.f, 175.f)
	};

	for (auto pos : positions) {
		LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::SwitchableOn, dynamic_cast<LevelScreen*>(m_screen));
		tile->init(properties);
		tile->setPosition(pos);
		tile->loadResources();
		tile->loadAnimation(7);
		tile->setPosition(pos);
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		m_screen->addObject(tile);
		m_puzzleBlocks.push_back(tile);
	}
}

void VeliusBoss::setupDivinePuzzle() {
	LevelTileProperties properties;

	auto positions = {
		sf::Vector2f(720.f, 350.f),
		sf::Vector2f(600.f, 175.f),
		sf::Vector2f(700.f, 175.f)
	};

	for (auto pos : positions) {
		LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::SwitchableOn, dynamic_cast<LevelScreen*>(m_screen));
		tile->init(properties);
		tile->setPosition(pos);
		tile->loadResources();
		tile->loadAnimation(7);
		tile->setPosition(pos);
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		m_screen->addObject(tile);
		m_puzzleBlocks.push_back(tile);
	}
}

void VeliusBoss::clearPuzzleBlocks() {
	for (auto block : m_puzzleBlocks) {
		block->setDisposed();
	}
	m_puzzleBlocks.clear();
}

void VeliusBoss::loadAnimation(int skinNr) {
	int width = 120;
	int height = 150;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 115.f));
	setSpriteOffset(sf::Vector2f(-50.f, -35.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
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

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(12 * width, 0, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// the "throwning" spell
	Animation* castingAnimation = new Animation(sf::seconds(0.15f));
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->addFrame(sf::IntRect(14 * width, 0, width, height));
	castingAnimation->addFrame(sf::IntRect(15 * width, 0, width, height));
	castingAnimation->addFrame(sf::IntRect(16 * width, 0, width, height));
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	// the shackle
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(13 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// the crystal
	Animation* casting3Animation = new Animation(sf::seconds(0.1f));
	casting3Animation->setSpriteSheet(tex);
	casting3Animation->addFrame(sf::IntRect(10 * width, 0, width, height));
	casting3Animation->setLooped(false);

	addAnimation(GameObjectState::Casting3, casting3Animation);

	// the come at me, brah
	Animation* comeAnimation = new Animation(sf::seconds(0.1f));
	comeAnimation->setSpriteSheet(tex);
	comeAnimation->addFrame(sf::IntRect(11 * width, 0, width, height));
	comeAnimation->setLooped(false);

	addAnimation(GameObjectState::Broken, comeAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadComponents();
}

MovingBehavior* VeliusBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new VeliusBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* VeliusBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&VeliusBoss::handleAttackInput, this));
	return behavior;
}

void VeliusBoss::loadComponents() {
	// component: light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, 0.f),
		sf::Vector2f(100.f, 100.f), 0.6f), this));

	loadBlockingParticles();
}

void VeliusBoss::loadBlockingParticles() {
	// particles
	ParticleComponentData data;
	data.particleCount = 200;
	data.isAdditiveBlendMode = true;
	data.emitRate = 0.f;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;

	// Generators
	auto spawner = new particles::CircleSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->radius = sf::Vector2f(60.f, 100.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 15.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(100, 40, 200, 200);
	colGen->maxStartCol = sf::Color(150, 100, 255, 200);
	colGen->minEndCol = sf::Color(0, 0, 0, 0);
	colGen->maxEndCol = sf::Color(150, 100, 200, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 1.f;
	velGen->maxStartSpeed = 2.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 0.5f;
	data.timeGen = timeGen;

	m_blockingBubble = new ParticleComponent(data, this);
	m_blockingBubble->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_blockingBubble);

	clearBlocking();
}

float VeliusBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

bool VeliusBoss::isBlocked() const {
	return m_isClones || m_isCalling || m_ray;
}

std::string VeliusBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_velius.png";
}