#include "Level/Enemies/NekomataEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/ExtendedWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Nekomata_blue, NekomataEnemy)

void NekomataEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	loot.insert({ "fo_rawmeat", 1 });
	gold = rand() % 10 + 2;
}

NekomataEnemy::NekomataEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Nekomata_blue);
}

void NekomataEnemy::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.resistanceLight = -30;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Shadow);
}

void NekomataEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 100;
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f NekomataEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(30.f, 20.f);
}

MovingBehavior* NekomataEnemy::createMovingBehavior() {
	ExtendedWalkingBehavior* behavior = new ExtendedWalkingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(600.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(80.f);
	behavior->setFightAnimationTime(sf::milliseconds(6 * 80));
	behavior->setFightStartTime(sf::milliseconds(1 * 80));
	behavior->setJumpStartTime(sf::milliseconds(3 * 80));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* NekomataEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&NekomataEnemy::handleAttackInput, this));
	return behavior;
}

void NekomataEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < m_enemyAttackingBehavior->getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void NekomataEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 130.f, 70.f));
	setSpriteOffset(sf::Vector2f(-10.f, -11.f));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));

	for (int i = 0; i < 12; i++) {
		walkingAnimation.addFrame(sf::IntRect(0, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		jumpingAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation jumpingStartAnimation(sf::seconds(0.08f));
	jumpingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 1; i < 4; i++) {
		jumpingStartAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping_start, jumpingStartAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		idleAnimation.addFrame(sf::IntRect(300, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation fightingStartAnimation(sf::seconds(0.08f));
	fightingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		fightingStartAnimation.addFrame(sf::IntRect(450, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Fighting_start, fightingStartAnimation);

	Animation fightingAnimation(sf::seconds(0.08f));
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	fightingAnimation.addFrame(sf::IntRect(450, 1 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 3 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		deadAnimation.addFrame(sf::IntRect(600, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

int NekomataEnemy::getMentalStrength() const {
	return 3;
}


