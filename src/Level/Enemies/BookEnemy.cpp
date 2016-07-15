#include "Level/Enemies/BookEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Book, BookEnemy)

void BookEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 5;
}

void BookEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 2 + 1;
}

BookEnemy::BookEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
	load();
}

void BookEnemy::loadAttributes() {
	m_attributes.setHealth(30);
	m_attributes.resistancePhysical = 5;
	m_attributes.resistanceIce = 10;
	m_attributes.resistanceShadow = 10;
	m_attributes.resistanceLight = 10;
	m_attributes.calculateAttributes();
}

void BookEnemy::loadSpells() {
	SpellData pageSpell = SpellData::getSpellData(SpellID::RotatingProjectile);
	pageSpell.isDynamicTileEffect = false;
	pageSpell.fightingTime = sf::seconds(0.f);
	pageSpell.castingTime = sf::seconds(0.f);
	pageSpell.castingAnimation = GameObjectState::VOID;
	pageSpell.fightAnimation = GameObjectState::VOID;
	pageSpell.speed = 400.f;

	m_spellManager->addSpell(pageSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* BookEnemy::createMovingBehavior(bool asAlly) {
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

AttackingBehavior* BookEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&BookEnemy::handleAttackInput, this));
	return behavior;
}

void BookEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 600.f) {
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

sf::Time BookEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 2 + 1));
}

sf::Time BookEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void BookEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 70.f, 50.f));
	setSpriteOffset(sf::Vector2f(-20.f, -7.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	flyingAnimation->addFrame(sf::IntRect(0, 0, 100, 64));
	flyingAnimation->addFrame(sf::IntRect(100, 0, 100, 64));
	flyingAnimation->addFrame(sf::IntRect(200, 0, 100, 64));
	flyingAnimation->addFrame(sf::IntRect(300, 0, 100, 64));
	flyingAnimation->addFrame(sf::IntRect(200, 0, 100, 64));
	flyingAnimation->addFrame(sf::IntRect(100, 0, 100, 64));

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 0, 100, 64));
	idleAnimation->addFrame(sf::IntRect(100, 0, 100, 64));
	idleAnimation->addFrame(sf::IntRect(200, 0, 100, 64));
	idleAnimation->addFrame(sf::IntRect(300, 0, 100, 64));
	idleAnimation->addFrame(sf::IntRect(200, 0, 100, 64));
	idleAnimation->addFrame(sf::IntRect(100, 0, 100, 64));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(300, 0, 100, 64));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void BookEnemy::setDead() {
	m_boundingBox.width = 70.f;
	m_boundingBox.height = 25.f;
	setSpriteOffset(sf::Vector2f(-20.f, -12.f));
	Enemy::setDead();
}

std::string BookEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_book.png";
}

