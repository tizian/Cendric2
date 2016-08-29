#include "Level/Enemies/PrisonerEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Prisoner, PrisonerEnemy)

PrisonerEnemy::PrisonerEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
}

void PrisonerEnemy::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistancePhysical = 2;
	m_attributes.calculateAttributes();
}

void PrisonerEnemy::loadSpells() {
	SpellData chop = SpellData::getSpellData(SpellID::Chop);
	chop.damage = 10;
	
	m_spellManager->addSpell(chop);
	m_spellManager->setCurrentSpell(0);

	setAlly(sf::Time::Zero);
}

float PrisonerEnemy::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

void PrisonerEnemy::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void PrisonerEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
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

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(15 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* PrisonerEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AllyWalkingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* PrisonerEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;

	behavior = new AllyBehavior(this);
	behavior->setAggroRange(20.f);
	behavior->setAttackInput(std::bind(&PrisonerEnemy::handleAttackInput, this));
	return behavior;
}

std::string PrisonerEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_prisoner.png";
}
