#include "Level/Enemies/GargoyleEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Gargoyle, GargoyleEnemy)

void GargoyleEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 20 + 8;
	loot.insert({ "fo_lesserhealingpotion", 1 });
	loot.insert({ "mi_gargoyle_dust", 1 });
}

void GargoyleEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 5 + 3;
}

GargoyleEnemy::GargoyleEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void GargoyleEnemy::update(const sf::Time& frameTime) {
	updateTime(m_attackWaitTime, frameTime);
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
	m_attributes.setHealth(500);
	m_attributes.resistancePhysical = 40;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceLight = -10;
	m_attributes.resistanceIce = 30;
	m_attributes.resistanceShadow = -10;
	m_attributes.calculateAttributes();
}

void GargoyleEnemy::loadSpells() {
	m_spellManager->clearSpells();

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 10 + m_spellAdditionalDamage / 2;
	fireBallSpell.damagePerSecond = 10 + m_spellAdditionalDamage / 2;
	fireBallSpell.duration = sf::seconds(1.f);
	fireBallSpell.cooldown = sf::seconds(1.f);
	fireBallSpell.speed = 300.f;
	fireBallSpell.count = m_spellCount;
	fireBallSpell.isStunning = true;
	fireBallSpell.ccStrength = m_spellStrength;
	fireBallSpell.isDynamicTileEffect = !m_isSummoned;
	fireBallSpell.fightingTime = sf::milliseconds(500);
	fireBallSpell.spellOffset = sf::Vector2f(30.f, 60.f);

	m_spellManager->addSpell(fireBallSpell);
	m_maxSpell = 0;

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

		m_maxSpell = 3;
	}

	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* GargoyleEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(150.f);
	behavior->setMaxVelocityYDown(150.f);
	behavior->setMaxVelocityYUp(150.f);
	behavior->setMaxVelocityX(150.f);
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
	
	if (m_attackWaitTime == sf::Time::Zero) {
		m_attackWaitTime = sf::seconds(2.f);
		if (m_maxSpell > 0) m_spellManager->setCurrentSpell(rand() % m_maxSpell); // random
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

sf::Time GargoyleEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4));
}

sf::Time GargoyleEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(5));
}

void GargoyleEnemy::loadAnimation(int skinNr) {
	m_animations.clear();
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 80.f));
	setSpriteOffset(sf::Vector2f(-127.f, -42.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		flyingAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		idleAnimation->addFrame(sf::IntRect(195 * i, 0, 195, 180));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// TODO: create other animations
	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 3; i++) {
		fightingAnimation->addFrame(sf::IntRect(195 * i, 180, 195, 180));
	}
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 10; i++) {
		deadAnimation->addFrame(sf::IntRect(195 * i, 360, 195, 180));
	}
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void GargoyleEnemy::setDead() {
	m_boundingBox.width = 138.f;
	m_boundingBox.height = 46.f;
	setSpriteOffset(sf::Vector2f(-25.f, -133.f));
	Enemy::setDead();
}

bool GargoyleEnemy::isSummoned() const {
	return m_isSummoned;
}

std::string GargoyleEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_gargoyle.png";
}

