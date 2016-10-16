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
	m_attributes.resistanceIce = 10;
	m_attributes.resistanceFire = -20;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void OozeEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(500);
	chopSpell.damage = 5;
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);
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

	behavior->setAggroRange(300.f);
	behavior->setAttackInput(std::bind(&OozeEnemy::handleAttackInput, this));
	return behavior;
}

void OozeEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 40.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
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
	idleAnimation->addFrame(sf::IntRect(0 * width, 0, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, 0, width, height));
	idleAnimation->addFrame(sf::IntRect(2 * width, 0, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* walkingAnimation = new Animation(sf::milliseconds(150));
	walkingAnimation->setSpriteSheet(tex);
	walkingAnimation->addFrame(sf::IntRect(3 * width, 0, width, height));
	walkingAnimation->addFrame(sf::IntRect(4 * width, 0, width, height));
	walkingAnimation->addFrame(sf::IntRect(5 * width, 0, width, height));
	walkingAnimation->addFrame(sf::IntRect(6 * width, 0, width, height));
	walkingAnimation->addFrame(sf::IntRect(7 * width, 0, width, height));
	walkingAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, 0, width, height));
	jumpingAnimation->setLooped(false);

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(10 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(11 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(12 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(13 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(14 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(15 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(16 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(17 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(18 * width, 0, width, height));
	deadAnimation->addFrame(sf::IntRect(19 * width, 0, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

std::string OozeEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_ooze.png";
}
