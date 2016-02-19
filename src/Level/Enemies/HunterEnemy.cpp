#include "Level/Enemies/HunterEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Hunter, HunterEnemy)

void HunterEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	// nop
}

HunterEnemy::HunterEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Hunter);
	setAlly(sf::Time::Zero);

	m_speechBubble = new SpeechBubble(this);
	m_speechBubble->setFloatingHeight(40.f);

	m_isImmortal = true;
}

void HunterEnemy::loadAttributes() {
	m_attributes.setHealth(400);
	m_attributes.resistanceIce = 30;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void HunterEnemy::loadSpells() {
	SpellData arrow = SpellData::getSpellData(SpellID::Projectile);
	arrow.damage = 10;
	arrow.duration = sf::seconds(2.f);
	arrow.damagePerSecond = 2;
	arrow.cooldown = sf::milliseconds(2000);

	m_spellManager->addSpell(arrow);

	m_spellManager->setCurrentSpell(0); // projectile
}

sf::Vector2f HunterEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(10.f, 20.f);
}

float HunterEnemy::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

void HunterEnemy::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	// should be something scripted here
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void HunterEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);

	m_speechBubble->update(frameTime);

	// will be scripted, only here for test purposes!
	if (m_speechBubbleState >= 4) return;

	if (m_speechBubbleState == 3 && getPosition().x > 1750) {
		m_speechBubbleState++;
		m_speechBubble->setText("KillRats");
	}
	else if (m_speechBubbleState == 2 && getPosition().x > 1500) {
		m_speechBubbleState++;
		m_speechBubble->setText("FindAWay");
	}
	else if (m_speechBubbleState == 1 && getPosition().x > 850) {
		m_speechBubbleState++;
		m_speechBubble->setText("SpikesAreBad");
	}
	else if (m_speechBubbleState == 0 && getPosition().x > 0) {
		m_speechBubbleState++;
		m_speechBubble->setText("DestroyBlocks");
	}
}

void HunterEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 95.f));
	setSpriteOffset(sf::Vector2f(-30.f, -25.f));

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_hunter));
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 100, 0, 100, 120));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_hunter));
	idleAnimation->addFrame(sf::IntRect(800, 0, 100, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_hunter));
	jumpingAnimation->addFrame(sf::IntRect(900, 0, 100, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_hunter));
	for (int i = 10; i < 14; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 100, 0, 100, 120));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_hunter));
	deadAnimation->addFrame(sf::IntRect(1400, 0, 100, 120));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* HunterEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AllyWalkingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(180.f);
	behavior->setFightAnimationTime(sf::milliseconds(4 * 70));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* HunterEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new AllyBehavior(this);
	behavior->setAggroRange(600.f);
	behavior->setAttackInput(std::bind(&HunterEnemy::handleAttackInput, this));
	return behavior;
}

int HunterEnemy::getMentalStrength() const {
	return 4;
}

void HunterEnemy::setDisposed() {
	Enemy::setDisposed();
	m_speechBubble->setDisposed();
}
void HunterEnemy::setScreen(Screen* screen) {
	Enemy::setScreen(screen);
	screen->addObject(m_speechBubble);
}

