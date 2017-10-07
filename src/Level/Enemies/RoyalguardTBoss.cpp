#include "Level/Enemies/RoyalguardTBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/RoyalguardTBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Royalguard_T, RoyalguardTBoss)

RoyalguardTBoss::RoyalguardTBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	RoyalguardBoss(level, screen) {
	loadWeapon();
}

void RoyalguardTBoss::loadAttributes() {
	m_attributes.setHealth(1500);
	m_attributes.resistanceFire = -20;
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceIce = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void RoyalguardTBoss::loadSpells() {
	// swirl
	SpellData swirl = SpellData::getSpellData(SpellID::Chop);
	swirl.damage = 20;
	swirl.cooldown = sf::seconds(1.f);
	swirl.fightingTime = sf::seconds(1.f);
	swirl.fightAnimation = GameObjectState::Fighting;
	swirl.isBlocking = true;

	m_spellManager->addSpell(swirl);

	m_spellManager->setCurrentSpell(0);
}

void RoyalguardTBoss::handleAttackInput() {
	// nop, handled over other functions
}

void RoyalguardTBoss::updateBossState(const sf::Time& frameTime) {
	if (m_isDead) return;
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
		m_weaponOffset = sf::Vector2f(27.f, 33.f);
		m_isWeaponVisible = true;
		break;
	case RoyalguardBossState::ChargingStart:
		m_bossState = RoyalguardBossState::Charging;
		m_weaponRotateType = WeaponRotateType::Fixed;
		m_weaponOffset = sf::Vector2f(29.f, 25.f);
		dynamic_cast<RoyalguardTBossMovingBehavior*>(getMovingBehavior())->setChargingDirection(normalized(m_mainChar->getCenter() - getCenter()));
		m_stateTime = sf::seconds(10.f);
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

void RoyalguardTBoss::loadAnimation(int skinNr) {
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

	// swirl
	Animation* fighting1Animation = new Animation();
	fighting1Animation->setSpriteSheet(tex);
	fighting1Animation->addFrame(sf::IntRect(20 * width, 0, width, height));
	fighting1Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fighting1Animation);

	// before charge
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(21 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// charge
	Animation* fighting2Animation = new Animation();
	fighting2Animation->setSpriteSheet(tex);
	fighting2Animation->addFrame(sf::IntRect(22 * width, 0, width, height));
	fighting2Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	// heal
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

MovingBehavior* RoyalguardTBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new RoyalguardTBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(600.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RoyalguardTBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&RoyalguardTBoss::handleAttackInput, this));
	return behavior;
}

std::string RoyalguardTBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_royalguard_t.png";
}

std::string RoyalguardTBoss::getWeaponTexturePath() const {
	return "res/assets/bosses/pike_t.png";
}

std::string RoyalguardTBoss::getDeathSoundPath() const {
	return "res/sound/mob/jeremy_death.ogg";
}
