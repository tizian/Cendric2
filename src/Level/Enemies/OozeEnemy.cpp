#include "Level/Enemies/OozeEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveCreepingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Ooze, OozeEnemy)

void OozeEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

void OozeEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 1;
}

OozeEnemy::OozeEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void OozeEnemy::loadAttributes() {
	m_attributes.setHealth(50);
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceIce = m_skinNr == 0 ? 10 : -50;
	m_attributes.resistanceFire = m_skinNr == 0 ? -20 : 1000;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void OozeEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(500);
	chopSpell.damage = 10;
	chopSpell.damageType = m_skinNr == 0 ? DamageType::Shadow : DamageType::Fire;
	chopSpell.damagePerSecond = 5;
	chopSpell.duration = sf::seconds(1.f);
	chopSpell.boundingBox = *getBoundingBox();
	chopSpell.fightingTime = sf::milliseconds(4 * 80);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);
	chopSpell.fightAnimation = GameObjectState::Idle;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* OozeEnemy::createMovingBehavior(bool asAlly) {
	CreepingBehavior* behavior = new AggressiveCreepingBehavior(this);
	
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYDown(400.f);
	behavior->setMaxVelocityYUp(400.f);
	behavior->setMaxVelocityX(50.f);

	return behavior;
}

AttackingBehavior* OozeEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior = new AggressiveBehavior(this);

	behavior->setAggroRange(150.f);
	behavior->setAttackInput(std::bind(&OozeEnemy::handleAttackInput, this));
	return behavior;
}

void OozeEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 40.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

void OozeEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));
	int width = 60;
	int height = 50;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::milliseconds(200));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0 * width, height * skinNr, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, height * skinNr, width, height));
	idleAnimation->addFrame(sf::IntRect(2 * width, height * skinNr, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, height * skinNr, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* walkingAnimation = new Animation(sf::milliseconds(150));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 3; i < 9; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, height * skinNr, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, height * skinNr, width, height));
	jumpingAnimation->setLooped(false);

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 20; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * width, height * skinNr, width, height));
	}
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

std::string OozeEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_ooze.png";
}

sf::Time OozeEnemy::getConfiguredRandomDecisionTime() const {
	return sf::seconds(4);
}

sf::Time OozeEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 2));
}

sf::Time OozeEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(2));
}