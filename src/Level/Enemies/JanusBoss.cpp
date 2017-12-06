#include "Level/Enemies/JanusBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Level/DynamicTiles/TorchTile.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Janus, JanusBoss)

const std::string JanusBoss::PARTICLE_TEX_PATH = "res/texture/particles/cloud.png";
const std::string JanusBoss::RED_SOUND = "res/sound/mob/janus_red.ogg";
const std::string JanusBoss::BLUE_SOUND = "res/sound/mob/janus_blue.ogg";
const sf::Time JanusBoss::PHASE_TIME = sf::seconds(10.f);

JanusBoss::JanusBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
	g_resourceManager->loadSoundbuffer(RED_SOUND, ResourceType::Level);
	g_resourceManager->loadSoundbuffer(BLUE_SOUND, ResourceType::Level);
	m_hasFirstPhaseStarted = false;
}

JanusBoss::~JanusBoss() {
	delete m_cloudPs;
}

void JanusBoss::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.resistanceIce = 200;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void JanusBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::milliseconds(17 * 50);
	chopSpell.cooldown = 2.f * chopSpell.activeDuration;
	chopSpell.boundingBox = sf::FloatRect(0, 0, 150, 150);
	chopSpell.spellOffset = sf::Vector2f(-75.f, 0.f);
	chopSpell.fightingTime = chopSpell.activeDuration;

	m_spellManager->addSpell(chopSpell);

	SpellData fireSpell = SpellData::getSpellData(SpellID::FireBall);
	fireSpell.damage = 20;
	fireSpell.skinNr = 5;
	fireSpell.cooldown = sf::seconds(0);
	fireSpell.spellOffset = sf::Vector2f(0.f, -20.f);
	fireSpell.count = 1;
	fireSpell.speed = 300;
	fireSpell.castingAnimation = GameObjectState::Casting2;
	fireSpell.fightAnimation = GameObjectState::Fighting2;
	fireSpell.castingTime = sf::milliseconds(12 * 80);
	fireSpell.fightingTime = sf::milliseconds(500);
	fireSpell.isBlocking = true;

	m_spellManager->addSpell(fireSpell);

	SpellData iceSpell = SpellData::getSpellData(SpellID::IceBall);
	iceSpell.damage = 20;
	iceSpell.skinNr = 1;
	iceSpell.cooldown = sf::seconds(0);
	iceSpell.spellOffset = sf::Vector2f(0.f, -20.f);
	iceSpell.count = 1;
	iceSpell.speed = 300;
	iceSpell.castingAnimation = GameObjectState::Casting3;
	iceSpell.fightAnimation = GameObjectState::Fighting3;
	iceSpell.castingTime = sf::milliseconds(12 * 80);
	iceSpell.fightingTime = sf::milliseconds(500);
	iceSpell.isBlocking = true;

	m_spellManager->addSpell(iceSpell);

	fireSpell.damage = 30;
	fireSpell.count = 1;
	fireSpell.cooldown = sf::seconds(5.f);
	fireSpell.isDynamicTileEffect = false;

	m_spellManager->addSpell(fireSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void JanusBoss::update(const sf::Time& frameTime) {
	loadTorches();
	m_cloudPs->update(frameTime);
	updateFirstPhase();
	updateTime(m_timeUntilPhase, frameTime);
	updateTorchStatus(frameTime);
	Boss::update(frameTime);
}

void JanusBoss::render(sf::RenderTarget& target) {
	m_cloudPs->render(target);
	Boss::render(target);
	m_mask.render(target);
}

void JanusBoss::updateFirstPhase() {
	if (m_hasFirstPhaseStarted) return;

	// a small but important piece of code to hinder people from one-cycling the boss
	if (m_attributes.currentHealthPoints < 0.75f * m_attributes.maxHealthPoints) {
		m_phase = TorchSpell1;
		m_enemyMovingBehavior->resetMovingTarget();
		m_hasFirstPhaseStarted = true;
	}
}

void JanusBoss::handleAttackInput() {
	switch (m_phase) {
	case Chop:
	default:
		m_spellManager->setCurrentSpell(3);

		if (m_enemyAttackingBehavior->distToTarget() < 120.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(m_mainChar);
		return;
	case ToTorch:
		return;
	case TorchSpell1:
		m_spellManager->setCurrentSpell(2);
		for (auto t : m_blueTorchTiles) {
			m_spellManager->executeCurrentSpell(t->getCenter(), true);
		}
		m_phase = TorchSpell2;
		m_hasLitTorches = false;
		return;
	case TorchSpell2:
		if (!isReady()) return;

		if (m_hasLitTorches) {
			m_phase = Chop;
			setEvil(true);
			return;
		}

		m_spellManager->setCurrentSpell(1);
		for (auto t : m_redTorchTiles) {
			m_spellManager->executeCurrentSpell(t->getCenter(), true);
		}
		m_hasLitTorches = true;
	}
}

void JanusBoss::setEvil(bool evil) {
	m_mask.setEvil(evil);
	g_resourceManager->playSound(evil ? RED_SOUND : BLUE_SOUND);
	m_isInvincible = evil;
	m_hasFirstPhaseStarted = true;
}

void JanusBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 150.f));
	setSpriteOffset(sf::Vector2f(-70.f, -65.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 200;
	int height = 200;

	Animation* walkingAnimation = new Animation(sf::milliseconds(80));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 6; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 4; i > 1; --i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation(sf::milliseconds(80));
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 4; i > 0; --i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 6; i < 11; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 10; i > 6; --i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(0, 0, width, height));

	Animation* fightingAnimation = new Animation(sf::milliseconds(50));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 4; i < 9; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}
	for (int i = 7; i > 0; --i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}
	for (int i = 0; i < 5; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// fire!
	Animation* castingFireAnimation = new Animation(sf::milliseconds(80));
	castingFireAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 12; ++i) {
		castingFireAnimation->addFrame(sf::IntRect(i * width, 3 * height, width, height));
	}

	addAnimation(GameObjectState::Casting2, castingFireAnimation);

	Animation* afterFireAnimation = new Animation();
	afterFireAnimation->setSpriteSheet(tex);
	afterFireAnimation->addFrame(sf::IntRect(11 * width, 3 * height, width, height));

	addAnimation(GameObjectState::Fighting2, afterFireAnimation);

	// ice!
	Animation* castingIceAnimation = new Animation(sf::milliseconds(80));
	castingIceAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 12; ++i) {
		castingIceAnimation->addFrame(sf::IntRect(i * width, 2 * height, width, height));
	}

	addAnimation(GameObjectState::Casting3, castingIceAnimation);

	Animation* afterIceAnimation = new Animation();
	afterIceAnimation->setSpriteSheet(tex);
	afterIceAnimation->addFrame(sf::IntRect(11 * width, 2 * height, width, height));

	addAnimation(GameObjectState::Fighting3, afterIceAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticles();
	loadCloudParticles();

	LightData data(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.2f), sf::Vector2f(150.f, 200.f), 0.9f);
	addComponent(new LightComponent(data, this));
}

void JanusBoss::loadTorches() {
	if (m_torchesLoaded) return;

	m_redTorchTiles.clear();
	m_blueTorchTiles.clear();
	for (GameObject* obj : *m_screen->getObjects(_DynamicTile)) {
		if (TorchTile* t = dynamic_cast<TorchTile*>(obj)) {
			if (t->getColor().compare("red") == 0) {
				m_redTorchTiles.push_back(t);
				t->setState(GameObjectState::Idle);
			}
			else {
				m_blueTorchTiles.push_back(t);
			}
		}
	}

	m_torchSpellPosition = getPosition();
	m_torchesLoaded = true;

	// initial values for janus
	m_phase = Vulnerable;
	m_timeUntilPhase = sf::seconds(3.f);
	setEvil(false);
}

MovingBehavior* JanusBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AggressiveWalkingBehavior(this);
	behavior->setApproachingDistance(80.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(400.f);
	behavior->setMaxVelocityX(70.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* JanusBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&JanusBoss::handleAttackInput, this));
	return behavior;
}

void JanusBoss::updateTorchStatus(const sf::Time& frameTime) {
	switch (m_phase) {
	case Chop: {
		m_isBlueTorchesFine = true;
		m_isRedTorchesFine = true;

		for (auto t : m_blueTorchTiles) {
			if (t->getGameObjectState() == GameObjectState::Idle) {
				m_isBlueTorchesFine = false;
				break;
			}
		}

		for (auto t : m_redTorchTiles) {
			if (t->getGameObjectState() == GameObjectState::Burning) {
				m_isRedTorchesFine = false;
				break;
			}
		}

		if (m_isBlueTorchesFine && m_isRedTorchesFine) {
			setEvil(false);
			// phase start
			m_phase = Vulnerable;
			m_timeUntilPhase = PHASE_TIME;
			return;
		}
		break;
	}
	case Vulnerable:
		if (m_timeUntilPhase == sf::Time::Zero) {
			m_enemyMovingBehavior->setMovingTarget(static_cast<int>(m_torchSpellPosition.x), static_cast<int>(m_torchSpellPosition.y));
			m_phase = ToTorch;
		}
		break;
	case ToTorch:
		if (abs(getCenter().x - m_torchSpellPosition.x) < 10.f) {
			m_phase = TorchSpell1;
			m_enemyMovingBehavior->resetMovingTarget();
		}
		break;
	case TorchSpell1:
	case TorchSpell2:
	default:
		break;
	}
}

void JanusBoss::setPosition(const sf::Vector2f& pos) {
	Boss::setPosition(pos);
	m_mask.setPosition(pos + sf::Vector2f(isFacingRight() ? 10.f : 9.f, -5.f));
	if (m_cloudSpawner) m_cloudSpawner->center = pos + sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height - 30.f);
}

sf::Time JanusBoss::getConfiguredWaitingTime() const {
	return sf::seconds(0);
}

float JanusBoss::getConfiguredDistanceToHPBar() const {
	return 60.f;
}

std::string JanusBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_janus.png";
}

void JanusBoss::loadCloudParticles() {
	g_resourceManager->loadTexture(PARTICLE_TEX_PATH, ResourceType::Level);

	sf::Texture* tex = g_resourceManager->getTexture(PARTICLE_TEX_PATH);
	if (tex == nullptr) return;

	m_cloudPs = new particles::TextureParticleSystem(100, g_resourceManager->getTexture(PARTICLE_TEX_PATH));
	m_cloudPs->emitRate = 40.f;

	// Generators
	auto posGen = m_cloudPs->addSpawner<particles::DiskSpawner>();
	posGen->radius = 30.f;
	m_cloudSpawner = posGen;

	auto sizeGen = m_cloudPs->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 40.f;

	auto colGen = m_cloudPs->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(200, 200, 200, 200);
	colGen->maxStartCol = sf::Color(210, 200, 210, 200);
	colGen->minEndCol = sf::Color(100, 100, 100, 0);
	colGen->maxEndCol = sf::Color(110, 100, 110, 0);

	auto velGen = m_cloudPs->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 0.f;
	velGen->maxStartSpeed = 2.f;

	auto timeGen = m_cloudPs->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_cloudPs->addUpdater<particles::TimeUpdater>();
	m_cloudPs->addUpdater<particles::ColorUpdater>();
	m_cloudPs->addUpdater<particles::EulerUpdater>();
	m_cloudPs->addUpdater<particles::SizeUpdater>();
}

//////////////////////////////////////////////////////////////
//// JANUS MASK //////////////////////////////////////////////
//////////////////////////////////////////////////////////////

JanusBossMask::JanusBossMask() {
	load();
}

void JanusBossMask::setPosition(const sf::Vector2f& pos) {
	m_sprite.setPosition(pos);
}

void JanusBossMask::setEvil(bool isEvil) {
	m_sprite.setTextureRect(sf::IntRect(isEvil ? 0 : 41, 0, 41, 40));
}

void JanusBossMask::render(sf::RenderTarget& target) {
	target.draw(m_sprite);
}

void JanusBossMask::load() {
	g_resourceManager->loadTexture(getSpritesheetPath(), ResourceType::Level);

	sf::Texture* tex = g_resourceManager->getTexture(getSpritesheetPath());
	if (tex == nullptr) return;

	m_sprite.setTexture(*tex);
	setEvil(true);
}

std::string JanusBossMask::getSpritesheetPath() const {
	return "res/texture/bosses/spritesheet_janus_mask.png";
}