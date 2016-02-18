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
	arrow.damage = 50;
	arrow.duration = sf::seconds(2.f);
	arrow.damagePerSecond = 10;
	arrow.cooldown = sf::milliseconds(2000);

	m_spellManager->addSpell(arrow);

	m_spellManager->setCurrentSpell(0); // projectile
}

sf::Vector2f HunterEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(10.f, 20.f);
}

void HunterEnemy::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	// should be something scripted here
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void HunterEnemy::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_speechBubble == nullptr) return;
	m_speechBubble->render(target);
}

void HunterEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_speechBubble == nullptr) return;

	m_speechBubble->update(frameTime);

	// will be scripted, only here for test purposes!
	if (m_speechBubbleState >= 4) return;

	if (m_speechBubbleState == 3 && getPosition().x > 1750) {
		m_speechBubble++;
		m_speechBubble->setText("KillRats");
	}
	else if (m_speechBubbleState == 2 && getPosition().x > 1500) {
		m_speechBubble++;
		m_speechBubble->setText("FindAWay");
	}
	else if (m_speechBubbleState == 1 && getPosition().x > 850) {
		m_speechBubble++;
		m_speechBubble->setText("SpikesAreBad");
	}
	else if (m_speechBubbleState == 0 && getPosition().x > 0) {
		m_speechBubble++;
		m_speechBubble->setText("DestroyBlocks");
	}
}

void HunterEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 90.f));
	setSpriteOffset(sf::Vector2f(-25.f, -30.f));

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

	//m_speechBubble = new SpeechBubble(this);
}

MovingBehavior* HunterEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AllyWalkingBehavior(this);

	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(150.f);
	behavior->setFightAnimationTime(sf::milliseconds(4 * 70));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* HunterEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new AllyBehavior(this);
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&HunterEnemy::handleAttackInput, this));
	return behavior;
}

int HunterEnemy::getMentalStrength() const {
	return 4;
}

void HunterEnemy::setDisposed() {
	Enemy::setDisposed();
	delete m_speechBubble;
	m_speechBubble = nullptr;
}
void HunterEnemy::setScreen(Screen* screen) {
	Enemy::setScreen(screen);
}

