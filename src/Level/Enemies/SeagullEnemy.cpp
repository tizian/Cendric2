#include "Level/Enemies/SeagullEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Seagull, SeagullEnemy)

void SeagullEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "mi_feather", rand() % 3 + 1 });
	loot.insert({ "fo_rawchicken", 1 });
	if (rand() % 2 == 0)
		loot.insert({ "fo_egg", 1 });
	loot.insert({ "mi_feather", rand() % 3 + 1 });
}

void SeagullEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	int feathers = rand() % 3;
	if (feathers > 0)
		loot.insert({ "mi_feather", feathers });
}

SeagullEnemy::SeagullEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void SeagullEnemy::loadAttributes() {
	m_attributes.setHealth(20);
	m_attributes.calculateAttributes();
}

void SeagullEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.damagePerSecond = 5;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);
	chopSpell.fightingTime = sf::milliseconds(3 * 100);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* SeagullEnemy::createMovingBehavior(bool asAlly) {
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

AttackingBehavior* SeagullEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(400.f);
	behavior->setAttackInput(std::bind(&SeagullEnemy::handleAttackInput, this));
	return behavior;
}

void SeagullEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		} 
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
}

sf::Time SeagullEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 3 + 1));
}

sf::Time SeagullEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 6 + 2));
}

void SeagullEnemy::loadAnimation(int skinNr) {
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

std::string SeagullEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_seagull.png";
}


