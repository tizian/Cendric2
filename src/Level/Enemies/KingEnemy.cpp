#include "Level/Enemies/KingEnemy.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::King, KingEnemy)

KingEnemy::KingEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
	m_isImmortal = true;
}

void KingEnemy::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.calculateAttributes();
}

void KingEnemy::loadSpells() {
	SpellData telekinesis = SpellData::getSpellData(SpellID::Telekinesis);
	telekinesis.fightingTime = sf::milliseconds(500);
	telekinesis.castingTime = sf::milliseconds(300);
	telekinesis.isBlocking = true;
	telekinesis.range = 500;
	telekinesis.activeDuration = sf::seconds(telekinesis.range / telekinesis.speed);
	
	m_spellManager->addSpell(telekinesis);
	m_spellManager->setCurrentSpell(0);

	setAlly(sf::Time::Zero);
}

float KingEnemy::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

void KingEnemy::handleAttackInput() {
	return;
}

void KingEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	int size = 120;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * size, 0, size, size));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * size, 0, size, size));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * size, 0, size, size));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * size, 0, size, size));
	}
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 14; i > 9; --i) {
		fightingAnimation->addFrame(sf::IntRect(i * size, 0, size, size));
	}
	fightingAnimation->setLooped(false);
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(14 * size, 0, size, size));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* KingEnemy::createMovingBehavior(bool asAlly) {
	auto behavior = new AllyWalkingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDropAlways(true);
	behavior->setReplaceDistance(300.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* KingEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new AllyBehavior(this);
	behavior->setAggroRange(0.f);
	behavior->setAttackInput(std::bind(&KingEnemy::handleAttackInput, this));
	return behavior;
}

int KingEnemy::getMentalStrength() const {
	return 4;
}

std::string KingEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_king.png";
}
