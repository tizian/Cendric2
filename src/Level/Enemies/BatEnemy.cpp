#include "Level/Enemies/BatEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Bat, BatEnemy)

void BatEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 2;
	if (rand() % 2 > 0)
		loot.insert({ "mi_teeth", 1 });
}

void BatEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 2 + 1;
}

BatEnemy::BatEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void BatEnemy::loadAttributes() {
	m_attributes.setHealth(30);
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceShadow = 200;
	m_attributes.critical = 10;
	m_attributes.calculateAttributes();
}

void BatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 10;
	chopSpell.damagePerSecond = 4;
	chopSpell.duration = sf::seconds(3.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);
	chopSpell.spellOffset = sf::Vector2f(-25.f, 0.f);
	chopSpell.fightingTime = sf::milliseconds(7 * 100);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* BatEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
		behavior->setMaxVelocityYUp(100.f);
		behavior->setMaxVelocityYDown(200.f);
		behavior->setMaxVelocityX(100.f);
		unlockAnimation();
		setState(GameObjectState::Idle);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
		behavior->setApproachingDistance(10.f);
		behavior->setMaxVelocityYDown(0.f);
		behavior->setMaxVelocityYUp(0.f);
		behavior->setMaxVelocityX(0.f);
	}

	return behavior;
}

AttackingBehavior* BatEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	m_isHanging = !asAlly;
	behavior->setAggroRange(300.f);
	behavior->setAttackInput(std::bind(&BatEnemy::handleAttackInput, this));
	return behavior;
}

void BatEnemy::onHit(Spell* spell) {
	Enemy::onHit(spell);
	setActive();
}

void BatEnemy::setDead() {
	if (m_isDead) return;
	setActive();
	Enemy::setDead();
}

void BatEnemy::setActive() {
	if (m_isDead || !m_isHanging) {
		return;
	}
	m_movingBehavior->setMaxVelocityYUp(100.f);
	m_movingBehavior->setMaxVelocityYDown(200.f);
	m_movingBehavior->setMaxVelocityX(100.f);
	unlockAnimation();
	m_isHanging = false;
	setState(GameObjectState::Idle);
}

void BatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	setActive();
	if (m_enemyAttackingBehavior->distToTarget() < 30.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
		m_chasingTime = sf::Time::Zero;
		m_waitingTime = sf::seconds(1);
	}
}

sf::Time BatEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 1));
}

sf::Time BatEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void BatEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 40.f));
	setSpriteOffset(sf::Vector2f(-15.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	int width = 80;
	int height = 90;

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	flyingAnimation->addFrame(sf::IntRect(0 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(4 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(5 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(4 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(5 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(8 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(9 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(10 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(9 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(10 * width, skinNr * height, width, height));
	fightingAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation(sf::seconds(10.f));
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(7 * width, skinNr * height, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* hangingAnimation = new Animation(sf::seconds(10.f));
	hangingAnimation->setSpriteSheet(tex);
	hangingAnimation->addFrame(sf::IntRect(6 * width, skinNr * height, width, height));
	hangingAnimation->setLooped(false);

	addAnimation(GameObjectState::Hanging, hangingAnimation);

	// initial values
	setState(GameObjectState::Hanging);
	setCurrentAnimation(getAnimation(GameObjectState::Hanging), false, true);
	playCurrentAnimation(true);

	m_isHanging = true;
}

std::string BatEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_bat.png";
}

std::string BatEnemy::getDeathSoundPath() const {
	return "res/sound/mob/bat_death.ogg";
}