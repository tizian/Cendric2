#include "Level/Enemies/YashaBoss.h"
#include "Level/Enemies/YashaBossAdd.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/YashaBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Spells/YashaRaiseTheDeadSpell.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"
#include "World/CustomParticleUpdaters.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Yasha, YashaBoss)

const sf::Vector2f YashaBoss::ROOM_MID = sf::Vector2f(650.f, 600.f);
const float YashaBoss::FADE_TIME = 2.f;
const float YashaBoss::REVIVE_CD = 10.f;
const std::string YashaBoss::SPELL_TEX_PATH = "res/texture/spells/spritesheet_spell_raisethedead.png";

const std::vector<sf::Vector2f> YashaBoss::ADD_LOCATIONS = { 
	sf::Vector2f(365.f, 485.f), 
	sf::Vector2f(865.f, 485.f), 
	sf::Vector2f(415.f, 185.f),
	sf::Vector2f(815.f, 185.f),
	sf::Vector2f(65.f, 335.f),
	sf::Vector2f(1165.f, 335.f),
	sf::Vector2f(65.f, 685.f),
	sf::Vector2f(1165.f, 685.f),
};

float YashaBoss::getConfiguredDistanceToHPBar() const {
	return 80.f;
}

YashaBoss::YashaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {

	g_resourceManager->loadTexture(SPELL_TEX_PATH, ResourceType::Level);
}

void YashaBoss::loadAttributes() {
	m_attributes.setHealth(1000);
	m_attributes.resistanceFire = 10000;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void YashaBoss::loadSpells() {
	SpellData explosionSpell = SpellData::getSpellData(SpellID::WindGust);
	explosionSpell.id = SpellID::Explosion;
	explosionSpell.activeDuration = sf::seconds(3.f);
	explosionSpell.damagePerSecond = 0;
	explosionSpell.damageType = DamageType::VOID;
	explosionSpell.strength = 1;
	explosionSpell.skinNr = 0;
	explosionSpell.cooldown = sf::seconds(10.f);
	explosionSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);
	explosionSpell.spellOffset = sf::Vector2f(-25.f, -120.f);
	explosionSpell.fightingTime = sf::seconds(3.f);
	explosionSpell.castingTime = sf::seconds(2.f);
	explosionSpell.castingAnimation = GameObjectState::Casting;
	explosionSpell.fightAnimation = GameObjectState::Fighting;
	explosionSpell.soundPaths.push_back("res/sound/spell/transformbeam.ogg");

	m_spellManager->addSpell(explosionSpell);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 20;
	fireBallSpell.damagePerSecond = 10;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::seconds(3.f);
	fireBallSpell.speed = 300.f;
	fireBallSpell.count = 2;
	fireBallSpell.spellOffset = sf::Vector2f(90.f, 0.f);
	fireBallSpell.fightingTime = sf::seconds(8 * 0.08f);
	fireBallSpell.castingTime = sf::seconds(5 * 0.08f);
	fireBallSpell.castingAnimation = GameObjectState::Casting2;
	fireBallSpell.fightAnimation = GameObjectState::Fighting2;

	m_spellManager->addSpell(fireBallSpell);

	m_spellManager->setCurrentSpell(1); // fireball
}

void YashaBoss::handleAttackInput() {
	switch (m_bossState)
	{
	case YashaBossState::Explosion:
		m_spellManager->setCurrentSpell(0);
		break;
	case YashaBossState::Fireballing:
		m_spellManager->setCurrentSpell(1);
		break;
	default:
	case YashaBossState::GotoStartCat:
	case YashaBossState::GotoExplosion:
	case YashaBossState::StartCat:
		return;
	}

	m_spellManager->executeCurrentSpell(m_mainChar);
}

void YashaBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 100.f, 90.f));
	setSpriteOffset(sf::Vector2f(-100.f, -160.f));
	const int width = 300;
	const int height = 250;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation(sf::seconds(0.08f));
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 13; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}
	idleAnimation->addFrame(sf::IntRect(2 * width, height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 7; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 2 * height, width, height));
	}
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(6 * width, 2 * height, width, height));
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* casting2Animation = new Animation(sf::seconds(0.08f));
	casting2Animation->setSpriteSheet(tex);
	for (int i = 0; i < 5; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * width, 3 * height, width, height));
	}

	addAnimation(GameObjectState::Casting2, casting2Animation);

	Animation* fighting2Animation = new Animation(sf::seconds(0.08f));
	fighting2Animation->setSpriteSheet(tex);
	for (int i = 5; i < 13; ++i) {
		fighting2Animation->addFrame(sf::IntRect(i * width, 3 * height, width, height));
	}

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticles();
	loadComponents();

	// init state
	m_bossState = YashaBossState::Fireballing;
	m_timeUntilNextState = sf::seconds(6.f);

	// init eyes
	m_eyes.setTexture(*tex);
	m_eyes.setTextureRect(sf::IntRect(7 * width, 2 * height, width, height));
}

MovingBehavior* YashaBoss::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;

	behavior = new YashaBossMovingBehavior(this);
	behavior->setApproachingDistance(200.f);
	behavior->setMaxVelocityYDown(100.f);
	behavior->setMaxVelocityYUp(150.f);
	behavior->setMaxVelocityX(200.f);
	return behavior;
}

AttackingBehavior* YashaBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&YashaBoss::handleAttackInput, this));
	return behavior;
}

void YashaBoss::update(const sf::Time& frameTime) {
	Boss::update(frameTime);
	updateBossState(frameTime);
	updateFading(frameTime);
}

void YashaBoss::renderAfterForeground(sf::RenderTarget& target) {
	if (m_bossState == YashaBossState::StartCat || m_bossState == YashaBossState::Cat) {
		target.draw(m_eyes);
	}
	Boss::renderAfterForeground(target);
}

void YashaBoss::updateBossState(const sf::Time& frameTime) {
	updateTime(m_timeUntilNextState, frameTime);
	if (m_timeUntilNextState > sf::Time::Zero) return;
	switch (m_bossState)
	{
	case YashaBossState::Fireballing:
		startBossState(explosionCount >= 2 ? YashaBossState::GotoStartCat :
			rand() % 2 == 0 ? YashaBossState::GotoStartCat : YashaBossState::GotoExplosion);
		break;
	case YashaBossState::Explosion:
		startBossState(YashaBossState::Fireballing);
		break;
	case YashaBossState::StartCat:
		spawnCats();
		startBossState(YashaBossState::Cat);
		m_reviveCD = sf::seconds(REVIVE_CD);
		break;
	case YashaBossState::Cat:
		checkRevive(frameTime);
		break;
	default:
	case YashaBossState::GotoStartCat:
	case YashaBossState::GotoExplosion:
		break;
	}
}

void YashaBoss::startBossState(YashaBossState state) {
	m_bossState = state;
	switch (m_bossState)
	{
	default:
	case YashaBossState::GotoExplosion:
	case YashaBossState::GotoStartCat:
		m_movingBehavior->setCollisionsEnabled(false);
		break;
	case YashaBossState::Explosion:
		m_timeUntilNextState = sf::seconds(6.f);
		explosionCount++;
		break;
	case YashaBossState::Fireballing:
		m_timeUntilNextState = sf::seconds(6.f);
		m_movingBehavior->setCollisionsEnabled(true);
		break;
	case YashaBossState::StartCat:
		m_fadeIn = false;
		m_timeUntilNextState = sf::seconds(3.f);
		break;
	case YashaBossState::Cat:
		explosionCount = 0;
		m_fadeIn = true;
		break;
	}
}

void YashaBoss::spawnCats() {
	int k = static_cast<int>(ADD_LOCATIONS.size());

	std::vector<int> indices;
	for (int i = 0; i < k; ++i) {
		indices.push_back(i);
	}

	for (int i = 0; i < 3; ++i) {
		int r = rand() % (k - i);
		sf::Vector2f location = ADD_LOCATIONS[indices[r]];
		indices.erase(indices.begin() + r);

		auto add = dynamic_cast<YashaBossAdd*>(dynamic_cast<LevelScreen*>(m_screen)->spawnEnemy(EnemyID::YashaAdd, location, i));
		add->setBoss(this);
		m_cats.push_back(add);
	}
}

void YashaBoss::checkRevive(const sf::Time& frameTime) {
	std::vector<Enemy*> deadCats;
	for (auto cat : m_cats) {
		if (cat->isDead()) {
			deadCats.push_back(cat);
		}
	}
	if (deadCats.empty()) return;
	if (deadCats.size() == m_cats.size()) {
		clearSpells(true);
		for (auto cat : m_cats) {
			cat->setDisposed();
		}
		m_cats.clear();
		startBossState(YashaBossState::Fireballing);
	}

	updateTime(m_reviveCD, frameTime);
	if (m_reviveCD > sf::Time::Zero) return;

	// revive a random cat. or at least try to.
	Enemy* target = deadCats.at(rand() % deadCats.size());
	SpellData data = SpellData::getSpellData(SpellID::RaiseTheDead);
	data.damageType = DamageType::Shadow;
	data.duration = sf::seconds(2.f);
	data.damage = 20;
	data.damagePerSecond = 10;
	data.range = 1000.f;
	data.activeDuration = sf::seconds(10.f);
	data.isColliding = false;
	data.spellOffset = sf::Vector2f(0.f, 0.f);
	data.skinNr = 1;
	data.isAlly = false;

	YashaRaiseTheDeadSpell* spell = new YashaRaiseTheDeadSpell();
	spell->load(data, this, target->getCenter());
	m_screen->addObject(spell);

	m_reviveCD = sf::seconds(REVIVE_CD);
}

void YashaBoss::updateFading(const sf::Time& frameTime) {
	if (m_fadeIn && m_currentDimming == 0.f) return;
	if (!m_fadeIn && m_currentDimming == 1.f) return;

	if (m_fadeIn) {
		m_currentDimming = std::max(0.f, m_currentDimming - frameTime.asSeconds() / FADE_TIME);
	}
	else {
		m_currentDimming = std::min(1.f, m_currentDimming + frameTime.asSeconds() / FADE_TIME);
	}

	m_level->setAmbientDimming(0.5f + m_currentDimming * 0.5f);
	m_level->setLightDimming(m_currentDimming);
}

void YashaBoss::setPosition(const sf::Vector2f& pos) {
	Boss::setPosition(pos);
	if (m_velGen) {
		m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - getBoundingBox()->height);
	}
	if (m_bossState == YashaBossState::StartCat || m_bossState == YashaBossState::Cat) {
		m_eyes.setPosition(m_movingBehavior->isFacingRight() ? pos + m_spriteOffset : pos + m_spriteOffset + sf::Vector2f(300.f, 0.f));
		m_eyes.setScale(m_movingBehavior->isFacingRight() ? 1.f : -1.f, m_eyes.getScale().y);
	}
}

sf::Time YashaBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

std::string YashaBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_yasha.png";
}

void YashaBoss::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 300.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	auto posGen = new particles::EllipseSpawner();
	posGen->radius = sf::Vector2f(m_boundingBox.width, m_boundingBox.height * 0.25f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 50.f;
	sizeGen->maxStartSize = 80.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	colGen->minStartCol = sf::Color(255, 160, 64, 0);
	colGen->maxStartCol = sf::Color(255, 160, 64, 0);
	colGen->minEndCol = sf::Color(255, 0, 0, 255);
	colGen->maxEndCol = sf::Color(255, 0, 0, 255);
	data.colorGen = colGen;

	m_velGen = new particles::AimedVelocityGenerator();
	m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	m_velGen->minStartSpeed = 40.f;
	m_velGen->maxStartSpeed = 80.f;
	data.velGen = m_velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, getBoundingBox()->height));
	addComponent(m_pc);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height),
		sf::Vector2f(m_boundingBox.width * 4.f, m_boundingBox.height * 2.f), 0.6f), this));
}

