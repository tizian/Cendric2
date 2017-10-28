#include "Level/Enemies/KingEnemy.h"
#include "Level/MOBBehavior/MovingBehaviors/NeutralWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/NeutralBehavior.h"
#include "GameObjectComponents/LightComponent.h"
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
	// nop, he hasn't got any skillz.
}

float KingEnemy::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

void KingEnemy::handleAttackInput() {
	// nop, this cowardly king doesn't fight.
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
	idleAnimation->addFrame(sf::IntRect(10 * size, 0, size, size));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * size, 0, size, size));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(1.f));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(8 * size, 0, size, size));
	fightingAnimation->setLooped(false);
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width, 0.f),
		sf::Vector2f(m_boundingBox.width * 2.f, m_boundingBox.width * 2.f), 0.6f), this));
}

MovingBehavior* KingEnemy::createMovingBehavior(bool asAlly) {
	auto behavior = new NeutralWalkingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(300.f);
	behavior->setDropAlways(true);
	behavior->setDefaultFightAnimation(sf::seconds(1.f), GameObjectState::Fighting);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* KingEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new NeutralBehavior(this);
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
