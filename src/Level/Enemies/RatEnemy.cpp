#include "Level/Enemies/RatEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Rat, RatEnemy)

void RatEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	loot.insert({ "fo_rawmeat", 1 });
	gold = 1;
}

RatEnemy::RatEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Rat);
}

void RatEnemy::loadAttributes() {
	m_attributes.setHealth(30);
	m_attributes.resistancePhysical = 10;
	m_attributes.calculateAttributes();
}

void RatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 5;
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

sf::Vector2f RatEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

MovingBehavior* RatEnemy::createMovingBehavior(bool asAlly) {
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
	behavior->setMaxVelocityYUp(400.f);
	behavior->setMaxVelocityX(100.f);
	behavior->setFightAnimationTime(sf::milliseconds(4 * 80));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RatEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(300.f);
	behavior->setAttackInput(std::bind(&RatEnemy::handleAttackInput, this));
	return behavior;
}

void RatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

bool RatEnemy::getFleeCondition() const {
	// what a cowardly rat
	return m_attributes.currentHealthPoints < m_attributes.maxHealthPoints / 3;
}

void RatEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	walkingAnimation->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	idleAnimation->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	jumpingAnimation->addFrame(sf::IntRect(150, 0, 50, 50));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.08f));
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	fightingAnimation->addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation->addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	deadAnimation->addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}
