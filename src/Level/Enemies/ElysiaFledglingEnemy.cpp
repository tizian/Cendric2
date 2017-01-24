#include "Level/Enemies/ElysiaFledglingEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Elysia_Fledgling, ElysiaFledglingEnemy)

void ElysiaFledglingEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "mi_feather", rand() % 3 + 1 });
	loot.insert({ "fo_rawchicken", 1 });
	if (rand() % 2 == 0)
		loot.insert({ "fo_egg", 1 });
}

void ElysiaFledglingEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	int feathers = rand() % 3;
	if (feathers > 0)
		loot.insert({ "mi_feather", feathers });
}

ElysiaFledglingEnemy::ElysiaFledglingEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void ElysiaFledglingEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceFire = -10;
	m_attributes.resistanceIce = 1000;
	m_attributes.resistancePhysical = 200;
	m_attributes.calculateAttributes();
}

void ElysiaFledglingEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::seconds(1);
	chopSpell.damage = 5;
	chopSpell.damagePerSecond = 6;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);
	chopSpell.fightingTime = sf::milliseconds(3 * 100);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);

	m_spellManager->addSpell(chopSpell);

	SpellData projectile = SpellData::getSpellData(SpellID::TargetingProjectile);
	projectile.cooldown = sf::seconds(5);
	projectile.activeDuration = sf::seconds(10.f);
	projectile.damageType = DamageType::Ice;
	projectile.damage = 20;
	projectile.damagePerSecond = 5;
	projectile.speed = 300;
	projectile.duration = sf::seconds(2.f);
	projectile.castingTime = sf::milliseconds(8 * 100);
	projectile.fightingTime = sf::milliseconds(3 * 100);
	projectile.spellOffset = sf::Vector2f(0.f, -40.f);

	m_spellManager->addSpell(projectile);

	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* ElysiaFledglingEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(100.f);
	behavior->setMaxVelocityX(100.f);
	return behavior;
}

AttackingBehavior* ElysiaFledglingEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(400.f);
	behavior->setAttackInput(std::bind(&ElysiaFledglingEnemy::handleAttackInput, this));
	return behavior;
}

void ElysiaFledglingEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
		m_spellManager->setCurrentSpell(0); // chop
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		}
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
	else {
		m_spellManager->setCurrentSpell(1); // targeting projectile
	}
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

sf::Time ElysiaFledglingEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 3 + 1));
}

sf::Time ElysiaFledglingEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 6 + 2));
}

void ElysiaFledglingEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 54.f));
	setSpriteOffset(sf::Vector2f(-5.f, -50.f));
	int width = 70;
	int height = 110;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	
	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for(int i = 0; i < 4; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 4; i < 7; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(7 * width, 0, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

std::string ElysiaFledglingEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_elysiafledgling.png";
}


