#include "Level/Enemies/SkeletonShieldEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton_Shield, SkeletonShieldEnemy)

void SkeletonShieldEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;
}

void SkeletonShieldEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonShieldEnemy::SkeletonShieldEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonShieldEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void SkeletonShieldEnemy::loadSpells() {
	// nop, doesn't have spells.
}

void SkeletonShieldEnemy::handleAttackInput() {
	// doesn't have attacks. It can kill the main char on touch though.
	if (isAlly() || m_mainChar->isDead()) return;

	const sf::FloatRect& mainCharBB = *m_mainChar->getBoundingBox();
	if (!mainCharBB.intersects(m_boundingBox)) return;

	if (m_isHiding) {
		m_mainChar->setDead();
		return;
	}

	float criticalX = getBoundingBox()->left + getBoundingBox()->width * 0.5f;
	if ((isFacingRight() && mainCharBB.left > criticalX) ||
		(!isFacingRight() && mainCharBB.left + mainCharBB.width * 0.5f < criticalX)) {
		m_mainChar->setDead();
	}
}

void SkeletonShieldEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 90.f));
	setSpriteOffset(sf::Vector2f(-40.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation(sf::milliseconds(70));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* hidingAnimation = new Animation();
	hidingAnimation->setSpriteSheet(tex);
	hidingAnimation->addFrame(sf::IntRect(15 * 120, skinNr * 120, 120, 120));
	hidingAnimation->setLooped(false);

	addAnimation(GameObjectState::Hiding, hidingAnimation);

	LightData data(sf::Vector2f(20.f, 20.f), sf::Vector2f(80.f, 100.f), 1.f);
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void SkeletonShieldEnemy::setDead() {
	Enemy::setDead();
	m_lightComponent->setVisible(false);
}

MovingBehavior* SkeletonShieldEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
		behavior->setMaxVelocityYUp(600.f);
		behavior->setMaxVelocityX(150.f);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
		behavior->setMaxVelocityYUp(0.f);
		behavior->setMaxVelocityX(0.f);
	}
	behavior->setDistanceToAbyss(80.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* SkeletonShieldEnemy::createAttackingBehavior(bool asAlly) {
	initializeHiding(asAlly);
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonShieldEnemy::handleAttackInput, this));
	return behavior;
}

void SkeletonShieldEnemy::initializeHiding(bool asAlly) {
	m_isHiding = !asAlly;
	if (m_isHiding) {
		setState(GameObjectState::Hiding);
		setCurrentAnimation(getAnimation(GameObjectState::Hiding), true, true);
		m_isInvincible = true;
	}
}

void SkeletonShieldEnemy::onHit(Spell* spell) {
	Enemy::onHit(spell);
	if (m_isDead || !m_isHiding) {
		return;
	}
	if (spell->getSpellID() != SpellID::Fear) {
		return;
	}
	m_isInvincible = false;
	unlockAnimation();
	m_isHiding = false;
	m_movingBehavior->setMaxVelocityYUp(600.f);
	m_movingBehavior->setMaxVelocityX(150.f);
	dynamic_cast<WalkingBehavior*>(m_movingBehavior)->calculateJumpHeight();
}

int SkeletonShieldEnemy::getMentalStrength() const {
	return 0;
}

std::string SkeletonShieldEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_skeletonshield.png";
}

