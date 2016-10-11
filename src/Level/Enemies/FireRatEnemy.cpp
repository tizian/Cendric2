#include "Level/Enemies/FireRatEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::FireRat, FireRatEnemy)

void FireRatEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "fo_rawmeat", 1 });
	if (rand() % 2 > 0)
		loot.insert({ "mi_teeth", 1 });
	gold = rand() % 4 + 1;
}

void FireRatEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "fo_rawmeat", 1 });
}

FireRatEnemy::FireRatEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

FireRatEnemy::~FireRatEnemy() {
	delete m_ps;
}

void FireRatEnemy::loadAttributes() {
	m_attributes.setHealth(40);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceFire = 1000;
	m_attributes.calculateAttributes();
}

void FireRatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 6;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);
	chopSpell.fightingTime = sf::milliseconds(4 * 80);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 6;
	fireBallSpell.damagePerSecond = 2;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 200.f;
	fireBallSpell.fightingTime = sf::milliseconds(4 * 80);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

void FireRatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < m_enemyAttackingBehavior->getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void FireRatEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	walkingAnimation->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(50, 0, 50, 50));
	idleAnimation->addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(150, 0, 50, 50));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.08f));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation->addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(350, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	switch (skinNr) {
	case 1:
		loadParticleSystem();
		m_lightComponent = new LightComponent(LightData(
			sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f),
			sf::Vector2f(200.f, 250.f), 0.6f), this);
		addComponent(m_lightComponent);
		m_isInvincible = true;
		m_isBurning = true;
		break;
	case 0:
	default:
		break;
	}

	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* FireRatEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}

	behavior->setDistanceToAbyss(20.f);
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYDown(400.f);

	if (m_isBurning && !asAlly) {
		behavior->setMaxVelocityYUp(0.f);
		behavior->setMaxVelocityX(0.f);
		behavior->calculateJumpHeight();
	}
	else {
		behavior->setMaxVelocityYUp(400.f);
		behavior->setMaxVelocityX(50.f);
	}
	behavior->calculateJumpHeight();

	return behavior;
}

AttackingBehavior* FireRatEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&FireRatEnemy::handleAttackInput, this));
	return behavior;
}

void FireRatEnemy::onHit(Spell* spell) {
	Enemy::onHit(spell);
	if (m_isDead || !m_isBurning) {
		return;
	}
	if (spell->getDamageType() != DamageType::Ice) {
		return;
	}
	m_isInvincible = false;
	m_movingBehavior->setMaxVelocityYUp(400.f);
	m_movingBehavior->setMaxVelocityX(50.f);
	dynamic_cast<WalkingBehavior*>(m_movingBehavior)->calculateJumpHeight();
	m_isBurning = false;
	delete m_ps;
	m_ps = nullptr;
	m_lightComponent->setVisible(false);
}

void FireRatEnemy::render(sf::RenderTarget& target) {
	if (m_ps != nullptr) {
		sf::RenderTarget& particleTarget = dynamic_cast<LevelScreen*>(getScreen())->getParticleRenderTexture();
		particleTarget.setView(target.getView());
		m_ps->render(particleTarget);
		GameObject::render(target);
	}
	else {
		Enemy::render(target);
	}
}

void FireRatEnemy::renderAfterForeground(sf::RenderTarget& target) {
	if (m_ps != nullptr) {
		return;
	}
	Enemy::renderAfterForeground(target);
}

void FireRatEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_ps != nullptr) {
		m_ps->update(frameTime);
	}
}

void FireRatEnemy::setPosition(const sf::Vector2f& pos) {
	Enemy::setPosition(pos);
	if (m_ps != nullptr) {
		m_particleSpawner->center.x = getPosition().x + 0.5f * getBoundingBox()->width;
		m_particleSpawner->center.y = getPosition().y + 0.5f * getBoundingBox()->height;
		m_velGenerator->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	}
}

int FireRatEnemy::getMentalStrength() const {
	return 1;
}

std::string FireRatEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_firerat.png";
}

void FireRatEnemy::loadParticleSystem() {
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME)->setSmooth(true);
	delete m_ps;
	m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 60.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y + 0.5f * getBoundingBox()->height);
	posGen->size = sf::Vector2f(40.f, 0.f);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 50.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 50.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();

	colGen->minStartCol = sf::Color(255, 160, 64);
	colGen->maxStartCol = sf::Color(255, 160, 64);
	colGen->minEndCol = sf::Color(255, 0, 0, 200);
	colGen->maxEndCol = sf::Color(255, 0, 0, 200);

	auto velGen = m_ps->addGenerator<particles::AimedVelocityGenerator>();
	velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	velGen->minStartSpeed = 40.f;
	velGen->maxStartSpeed = 80.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.8f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}
