#include "Level/Enemies/CrowEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Crow, CrowEnemy)

void CrowEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "mi_feather", 1 });
	gold = 2;
}

void CrowEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 2 + 1;
}

CrowEnemy::CrowEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
	load();
}

void CrowEnemy::loadAttributes() {
	m_attributes.setHealth(60);
	m_attributes.resistancePhysical = 10;
	m_attributes.calculateAttributes();
}

void CrowEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.damagePerSecond = 10;
	chopSpell.duration = sf::seconds(3.f);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);
	chopSpell.fightingTime = sf::milliseconds(3 * 100);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* CrowEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(100.f);
	behavior->setMaxVelocityX(100.f);
	return behavior;
}

AttackingBehavior* CrowEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(400.f);
	behavior->setAttackInput(std::bind(&CrowEnemy::handleAttackInput, this));
	return behavior;
}

void CrowEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		} 
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
}

sf::Time CrowEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 1));
}

sf::Time CrowEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void CrowEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 54.f));
	setSpriteOffset(sf::Vector2f(-5.f, -5.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	flyingAnimation->addFrame(sf::IntRect(0, 0, 70, 64));
	flyingAnimation->addFrame(sf::IntRect(70, 0, 70, 64));
	flyingAnimation->addFrame(sf::IntRect(140, 0, 70, 64));
	flyingAnimation->addFrame(sf::IntRect(210, 0, 70, 64));

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 0, 70, 64));
	idleAnimation->addFrame(sf::IntRect(70, 0, 70, 64));
	idleAnimation->addFrame(sf::IntRect(140, 0, 70, 64));
	idleAnimation->addFrame(sf::IntRect(210, 0, 70, 64));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(0, 64, 70, 64));
	fightingAnimation->addFrame(sf::IntRect(70, 64, 70, 64));
	fightingAnimation->addFrame(sf::IntRect(140, 64, 70, 64));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(0, 128, 70, 64));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

std::string CrowEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_crow.png";
}

