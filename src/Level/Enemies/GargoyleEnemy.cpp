#include "Level/Enemies/GargoyleEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Gargoyle, GargoyleEnemy)

void GargoyleEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	gold = rand() % 20 + 8;
	loot.insert({ "fo_lesserhealingpotion", 1 });
}

GargoyleEnemy::GargoyleEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Gargoyle);
}

void GargoyleEnemy::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_attackWaitTime, frameTime);
	Enemy::update(frameTime);
}

void GargoyleEnemy::setSummoned(int strength, int damage, int count, const sf::Time& ttl) {
	m_isSummoned = true;
	m_spellStrength = strength;
	m_spellCount = count;
	m_spellAdditionalDamage = damage;
	loadSpells();
	loadAnimation(m_spellStrength - 1);
	setAlly(ttl);
}

void GargoyleEnemy::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistancePhysical = 20;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceLight = -10;
	m_attributes.resistanceIce = 30;
	m_attributes.resistanceShadow = -10;
	m_attributes.calculateAttributes();
}

void GargoyleEnemy::loadSpells() {
	m_spellManager->clearSpells();
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 30 + m_spellAdditionalDamage;
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 10 + m_spellAdditionalDamage / 2;
	fireBallSpell.damagePerSecond = 10 + m_spellAdditionalDamage / 2;
	fireBallSpell.duration = sf::seconds(1.f);
	fireBallSpell.cooldown = sf::seconds(1.f);
	fireBallSpell.speed = 300.f;
	fireBallSpell.count = m_spellCount;
	fireBallSpell.isStunning = true;
	fireBallSpell.strength = m_spellStrength;
	fireBallSpell.isDynamicTileEffect = !m_isSummoned;

	m_spellManager->addSpell(fireBallSpell);
	m_maxSpell = 1;

	if (m_isSummoned) {
		fireBallSpell.skinNr = 1;
		fireBallSpell.damageType = DamageType::Ice;

		m_spellManager->addSpell(fireBallSpell);

		fireBallSpell.skinNr = 2;
		fireBallSpell.damageType = DamageType::Shadow;

		m_spellManager->addSpell(fireBallSpell);

		fireBallSpell.skinNr = 3;
		fireBallSpell.damageType = DamageType::Light;

		m_spellManager->addSpell(fireBallSpell);

		m_maxSpell = 4;
	}

	m_spellManager->setCurrentSpell(0);
}

sf::Vector2f GargoyleEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(20.f, 50.f);
}

MovingBehavior* GargoyleEnemy::createMovingBehavior() {
	FlyingBehavior* behavior = new FlyingBehavior(this);
	behavior->setApproachingDistance(60.f);
	behavior->setMaxVelocityYDown(150.f);
	behavior->setMaxVelocityYUp(150.f);
	behavior->setMaxVelocityX(150.f);
	behavior->setFightAnimationTime(sf::milliseconds(3 * 100));
	return behavior;
}

AttackingBehavior* GargoyleEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(400.f);
	behavior->setAttackInput(std::bind(&GargoyleEnemy::handleAttackInput, this));
	return behavior;
}

void GargoyleEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
		m_spellManager->setCurrentSpell(0); // chop
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 2) + 1);
		}
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 4));
		}
		return;
	}
	if (m_attackWaitTime == sf::Time::Zero) {
		m_attackWaitTime = sf::seconds(2.f);
		m_spellManager->setCurrentSpell(rand() % m_maxSpell + 1); // random
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

sf::Time GargoyleEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4));
}

sf::Time GargoyleEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(5));
}

void GargoyleEnemy::loadAnimation() {
	loadAnimation(0);
}

void GargoyleEnemy::loadAnimation(int skinNr) {
	m_animations.clear();
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 80.f));
	setSpriteOffset(sf::Vector2f(-127.f, -42.f));

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_gargoyle));
	for (int i = 0; i < 8; i++) {
		flyingAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_gargoyle));
	for (int i = 0; i < 8; i++) {
		idleAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// TODO: create other animations
	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_gargoyle));
	for (int i = 0; i < 8; i++) {
		fightingAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_gargoyle));
	for (int i = 0; i < 8; i++) {
		deadAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

bool GargoyleEnemy::isSummoned() const {
	return m_isSummoned;
}

