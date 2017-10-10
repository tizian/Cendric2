#include "Level/Enemies/LeeroyEnemy.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Leeroy, LeeroyEnemy)

LeeroyEnemy::LeeroyEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void LeeroyEnemy::loadAttributes() {
	m_attributes.setHealth(400);
	m_attributes.resistanceIce = 30;
	m_attributes.resistanceFire = -10;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void LeeroyEnemy::loadSpells() {
	SpellData chop = SpellData::getSpellData(SpellID::Chop);
	chop.boundingBox = sf::FloatRect(0.f, 0.f, 120.f, 80.f);
	chop.damage = 10;
	chop.damagePerSecond = 2;
	chop.cooldown = sf::seconds(2.f);
	chop.cooldown = sf::milliseconds(2000);
	chop.fightingTime = sf::milliseconds(500);
	chop.castingTime = sf::milliseconds(300);
	chop.isBlocking = true;
	
	m_spellManager->addSpell(chop);
	m_spellManager->setCurrentSpell(0); // chop

	setAlly(sf::Time::Zero);
}

float LeeroyEnemy::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

void LeeroyEnemy::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 80.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

void LeeroyEnemy::loadAnimation(int skinNr) {
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
	castingAnimation->addFrame(sf::IntRect(11 * size, 0, size, size));
	castingAnimation->addFrame(sf::IntRect(10 * size, 0, size, size));
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(11 * size, 0, size, size));
	fightingAnimation->addFrame(sf::IntRect(12 * size, 0, size, size));
	fightingAnimation->addFrame(sf::IntRect(15 * size, 0, 2 * size, size));
	fightingAnimation->addFrame(sf::IntRect(13 * size, 0, size, size));
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

MovingBehavior* LeeroyEnemy::createMovingBehavior(bool asAlly) {
	auto behavior = new AllyWalkingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(300.f);
	behavior->calculateJumpHeight();
	behavior->setReplaceDistance(10000.f);
	return behavior;
}

AttackingBehavior* LeeroyEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new AllyBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&LeeroyEnemy::handleAttackInput, this));
	return behavior;
}

int LeeroyEnemy::getMentalStrength() const {
	return 4;
}

std::string LeeroyEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_leeroy.png";
}
