#include "Level/Enemies/WolfEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Wolf, WolfEnemy)

void WolfEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "mi_wolf_fur", 1 });
	loot.insert({ "fo_rawmeat", 1 });
	gold = rand() % 6;
}

void WolfEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "fo_rawmeat", 1 });
	gold = rand() % 3;
}

WolfEnemy::WolfEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	load(EnemyID::Wolf);
}

void WolfEnemy::loadAttributes() {
	m_attributes.setHealth(60);
	m_attributes.resistancePhysical = 10;
	m_attributes.calculateAttributes();
}

void WolfEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 10;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 2;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(2000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 70, 100);
	chopSpell.spellOffset = sf::Vector2f(0, 30);
	chopSpell.fightingTime = sf::milliseconds(6 * 100);

	m_spellManager->addSpell(chopSpell);

	SpellData howlSpell = SpellData::getSpellData(SpellID::Buff);
	howlSpell.damage = 5;
	howlSpell.cooldown = sf::milliseconds(10000);
	howlSpell.fightAnimation = GameObjectState::Fighting2;
	howlSpell.isBlocking = true;
	howlSpell.fightingTime = sf::seconds(2.f);
	howlSpell.activeDuration = sf::seconds(2.f);
	howlSpell.duration = sf::seconds(5.f);
	howlSpell.soundPath = "res/sound/mob/wolfhowl.ogg";

	m_spellManager->addSpell(howlSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void WolfEnemy::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 140.f) {
		m_spellManager->setCurrentSpell(0);
	}
	else {
		m_spellManager->setCurrentSpell(1);
	}
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void WolfEnemy::loadAnimation() {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	setBoundingBox(sf::FloatRect(0.f, 0.f, 106.f, 55.f));
	setSpriteOffset(sf::Vector2f(-17.f, -45.f));

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 140, 0, 140, 100));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 100, 140, 100));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(0, 200, 140, 100));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* howlingAnimation = new Animation(sf::seconds(0.1f));
	howlingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 2; ++i) {
		howlingAnimation->addFrame(sf::IntRect(i * 140, 400, 140, 100));
	}
	howlingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting2, howlingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.1f));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 2; i < 5; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 140, 400, 140, 100));
	}
	for (int i = 4; i > 1; --i) {
		fightingAnimation->addFrame(sf::IntRect(i * 140, 400, 140, 100));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 3; i++) {
		deadAnimation->addFrame(sf::IntRect(i * 140, 300, 140, 100));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* WolfEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(80.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(150.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* WolfEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&WolfEnemy::handleAttackInput, this));
	return behavior;
}

int WolfEnemy::getMentalStrength() const {
	return 2;
}

std::string WolfEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_wolf.png";
}

