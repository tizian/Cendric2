#include "Level/Enemies/StonemanEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Stoneman, StonemanEnemy)

void StonemanEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;
}

StonemanEnemy::StonemanEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	load(EnemyID::Stoneman);
}

void StonemanEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceIce = 10;
	m_attributes.resistanceFire = 10;
	m_attributes.resistanceShadow = 10;
	m_attributes.calculateAttributes();
}

void StonemanEnemy::loadSpells() {

}

sf::Vector2f StonemanEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(10.f, 0.f);
}

void StonemanEnemy::handleAttackInput() {

}

void StonemanEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 88.f));
	setSpriteOffset(sf::Vector2f(-37.f, -32.f));

	Animation* walkingAnimation = new Animation(sf::seconds(0.12f));
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 100, 0, 100, 120));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	idleAnimation->addFrame(sf::IntRect(800, 0, 100, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	for (int i = 0; i < 6; ++i) {
		deadAnimation->addFrame(sf::IntRect(800 + i * 100, 0, 100, 120));
	}
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.12f));
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	fightingAnimation->addFrame(sf::IntRect(800, 0, 100, 120));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* jumpingAnimation = new Animation(sf::seconds(0.12f));
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	jumpingAnimation->addFrame(sf::IntRect(800, 0, 100, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* StonemanEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(30.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(0.f);
	behavior->setMaxVelocityX(80.f);
	behavior->setFightAnimationTime(sf::milliseconds(8 * 120));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* StonemanEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&StonemanEnemy::handleAttackInput, this));
	return behavior;
}

int StonemanEnemy::getMentalStrength() const {
	return 2;
}

