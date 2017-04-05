#include "Level/Enemies/MorgianaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Morgiana, MorgianaBoss)

MorgianaBoss::MorgianaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isAlwaysUpdate = true;
	m_isBoss = true;
}

void MorgianaBoss::loadAttributes() {
	m_attributes.setHealth(800);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceLight = -20;
	m_attributes.resistancePhysical = 10000;
	m_attributes.resistanceFire = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void MorgianaBoss::loadSpells() {
	// the ultimate chop
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 100;
	chopSpell.damagePerSecond = 1;
	chopSpell.cooldown = sf::seconds(5.f);
	chopSpell.isBlocking = true;
	chopSpell.fightingTime = sf::seconds(1.f);
	chopSpell.fightAnimation = GameObjectState::Fighting;
	chopSpell.castingTime = sf::seconds(0.6f);
	chopSpell.castingAnimation = GameObjectState::Casting;
	chopSpell.soundPath = "res/sound/mob/morgiana_hammer.ogg";
	chopSpell.isSoundLooping = false;

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // ultimative chop
}

void MorgianaBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void MorgianaBoss::loadAnimation(int skinNr) {
	int width = 120;
	int wideWidth = 180;
	int height = 150;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -60.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);
	
	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, 0, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(7 * width, 1 * height, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);
	
	Animation* castingAnimation = new Animation(sf::seconds(0.1f));
	castingAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 17; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 0; i < 2; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * wideWidth, 1 * height, wideWidth, height));
	}
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(1.f));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(3 * width, 1 * height, width, height));
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	for (int i = 4; i < 6; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * width, 1 * height, width, height));
	}
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	Animation* fighting2Animation = new Animation(sf::seconds(1.f));
	fighting2Animation->setSpriteSheet(tex);
	fighting2Animation->addFrame(sf::IntRect(6 * width, 1 * height, width, height));
	fighting2Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	Animation* casting3Animation = new Animation(sf::seconds(0.2f));
	casting3Animation->setSpriteSheet(tex);
	for (int i = 8; i < 10; ++i) {
		casting3Animation->addFrame(sf::IntRect(i * width, 1 * height, width, height));
	}
	casting3Animation->setLooped(false);

	addAnimation(GameObjectState::Casting3, casting3Animation);

	Animation* fighting3Animation = new Animation(sf::seconds(1.f));
	fighting3Animation->setSpriteSheet(tex);
	fighting3Animation->addFrame(sf::IntRect(10 * width, 1 * height, width, height));
	fighting3Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting3, fighting3Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* MorgianaBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new AggressiveWalkingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* MorgianaBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&MorgianaBoss::handleAttackInput, this));
	return behavior;
}

sf::Time MorgianaBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

float MorgianaBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

std::string MorgianaBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_morgiana.png";
}

std::string MorgianaBoss::getDeathSoundPath() const {
	return "res/sound/mob/morgiana_death.ogg";
}
