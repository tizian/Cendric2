#include "Level/Enemies/RoyalguardIBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/RoyalguardIBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Royalguard_I, RoyalguardIBoss)

RoyalguardIBoss::RoyalguardIBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	RoyalguardBoss(level, screen) {
	loadWeapon();
}

void RoyalguardIBoss::loadAttributes() {
	m_attributes.setHealth(1500);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceFire = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void RoyalguardIBoss::loadSpells() {
	// fireball
	SpellData fireball = SpellData::getSpellData(SpellID::FireBall);
	fireball.damage = 100;
	fireball.damagePerSecond = 1;
	fireball.cooldown = sf::seconds(5.f);
	fireball.isBlocking = true;
	fireball.castingTime = sf::seconds(2.f);
	fireball.fightingTime = sf::seconds(0.f);
	fireball.fightAnimation = GameObjectState::VOID;
	fireball.castingAnimation = GameObjectState::Fighting3;

	m_spellManager->addSpell(fireball);

	m_spellManager->setCurrentSpell(0);
}

void RoyalguardIBoss::handleAttackInput() {
	// nop
}

void RoyalguardIBoss::updateBossState(const sf::Time& frameTime) {
	if (m_isDead) return;
	return;
	updateTime(m_stateTime, frameTime);
	if (m_stateTime > sf::Time::Zero) return;
	switch (m_bossState)
	{
	default:
	case RoyalguardBossState::Waiting:
		if (dist(getCenter(), m_mainChar->getCenter()) > 50.f) {
			m_bossState = RoyalguardBossState::ChargingStart;
			m_stateTime = sf::seconds(1.5f);
			m_weaponRotateType = WeaponRotateType::ToMainChar;
		}
		else {
			m_bossState = RoyalguardBossState::Swirl;
			m_stateTime = sf::seconds(1.f);
			m_spellManager->setCurrentSpell(0);
			m_spellManager->executeCurrentSpell(m_mainChar);
			m_weaponRotateType = WeaponRotateType::Turn;
		}
		m_weaponOffset = sf::Vector2f(72.f, 93.f);
		m_isWeaponVisible = true;
		break;
	case RoyalguardBossState::ChargingStart:
		m_bossState = RoyalguardBossState::Charging;
		m_weaponRotateType = WeaponRotateType::Fixed;
		m_weaponOffset = sf::Vector2f(74.f, 85.f);
		break;
	case RoyalguardBossState::Charging:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		m_stateTime = sf::seconds(1.f);
		break;
	case RoyalguardBossState::Swirl:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		break;
	case RoyalguardBossState::Healing:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		m_stateTime = sf::seconds(1.f);
		m_other->revive();
		break;
	}
}

void RoyalguardIBoss::loadAnimation(int skinNr) {
	int width = 120;
	int wideWidth = 240;
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
	idleAnimation->addFrame(sf::IntRect(7 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(9 * width, 1 * height, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// chop down
	Animation* casting1Animation = new Animation(sf::seconds(0.1f));
	casting1Animation->setSpriteSheet(tex);
	casting1Animation->addFrame(sf::IntRect(10 * width, 0, width, height));
	casting1Animation->addFrame(sf::IntRect(11 * width, 0, width, height));
	casting1Animation->addFrame(sf::IntRect(12 * width, 0, wideWidth, height));
	casting1Animation->addFrame(sf::IntRect(14 * width, 0, wideWidth, height));
	casting1Animation->addFrame(sf::IntRect(16 * width, 0, wideWidth, height));
	casting1Animation->setLooped(false);

	addAnimation(GameObjectState::Casting, casting1Animation);

	Animation* fighting1Animation = new Animation();
	fighting1Animation->setSpriteSheet(tex);
	fighting1Animation->addFrame(sf::IntRect(16 * width, 0, wideWidth, height));
	fighting1Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fighting1Animation);

	// chop up
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(16 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(14 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(12 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(11 * width, 0, width, height));
	casting2Animation->addFrame(sf::IntRect(10 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	Animation* fighting2Animation = new Animation();
	fighting2Animation->setSpriteSheet(tex);
	fighting2Animation->addFrame(sf::IntRect(10 * width, 0, width, height));
	fighting2Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	// heal & cast fireball
	Animation* fighting3Animation = new Animation(sf::seconds(0.1f));
	fighting3Animation->setSpriteSheet(tex);
	fighting3Animation->addFrame(sf::IntRect(18 * width, 0, width, height));
	fighting3Animation->addFrame(sf::IntRect(19 * width, 0, width, height));
	fighting3Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting3, fighting3Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* RoyalguardIBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new RoyalguardIBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(0.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RoyalguardIBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&RoyalguardIBoss::handleAttackInput, this));
	return behavior;
}

std::string RoyalguardIBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_royalguard_i.png";
}

std::string RoyalguardIBoss::getWeaponTexturePath() const {
	return "res/assets/bosses/pike_i.png";
}

std::string RoyalguardIBoss::getDeathSoundPath() const {
	return "res/sound/mob/morgiana_death.ogg";
}

/////////////////////
// ROYALGUARD FIRE //
/////////////////////

const sf::Time RoyalguardFire::GRACE_TIME = sf::seconds(2.f);

RoyalguardFire::RoyalguardFire(bool isTop, LevelMovableGameObject* mainChar) {
	m_mainChar = mainChar;
}

void RoyalguardFire::update(const sf::Time& frameTime) {
	updateTime(m_graceTime, frameTime);
	if (m_graceTime > sf::Time::Zero) {
		return;
	}
	if (m_mainChar->getBoundingBox()->intersects(m_boundingBox)) {
		m_mainChar->setDead();
	}
}

void RoyalguardFire::loadParticles() {

}
