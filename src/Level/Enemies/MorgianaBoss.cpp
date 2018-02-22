#include "Level/Enemies/JeremyBoss.h"
#include "Level/Enemies/RoyBoss.h"
#include "Level/Enemies/MorgianaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"
#include "Steam/AchievementManager.h"

REGISTER_ENEMY(EnemyID::Boss_Morgiana, MorgianaBoss)

const sf::Time MorgianaBoss::BLOCKING_TIME = sf::seconds(3.f);

MorgianaBoss::MorgianaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isAlwaysUpdate = true;
	m_isBoss = true;
}

void MorgianaBoss::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (!m_isBlocking) return;
	updateTime(m_blockingTime, frameTime);
	if (m_blockingTime == sf::Time::Zero) {
		m_isBlocking = false;
		m_isInvincible = false;
	}
}

void MorgianaBoss::loadAttributes() {
	m_attributes.setHealth(400);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceLight = -20;
	m_attributes.resistanceShadow = -20;
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceFire = 100;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void MorgianaBoss::loadSpells() {
	// the ultimate chop
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 60;
	chopSpell.damagePerSecond = 1;
	chopSpell.cooldown = sf::seconds(3.f);
	chopSpell.boundingBox = sf::FloatRect(0.f, 0.f, 60.f, 80.f);
	chopSpell.isBlocking = true;
	chopSpell.fightingTime = sf::seconds(1.f);
	chopSpell.fightAnimation = GameObjectState::Fighting;
	chopSpell.castingTime = sf::seconds(0.6f);
	chopSpell.castingAnimation = GameObjectState::Casting;
	chopSpell.creatorSoundPaths = { "res/sound/mob/morgiana_hammer.ogg" };

	m_spellManager->addSpell(chopSpell);

	// block
	SpellData blockSpell = SpellData::getSpellData(SpellID::Buff);
	blockSpell.damage = 0;
	blockSpell.damageType = DamageType::VOID;
	blockSpell.isDynamicTileEffect = false;
	blockSpell.cooldown = sf::seconds(10.f);
	blockSpell.isBlocking = true;
	blockSpell.fightingTime = BLOCKING_TIME;
	blockSpell.fightAnimation = GameObjectState::Fighting2;
	blockSpell.castingTime = sf::seconds(0.6f);
	blockSpell.castingAnimation = GameObjectState::Casting2;

	m_spellManager->addSpell(blockSpell);

	// the shadow step ninja icy ambush from jeremy
	SpellData icyAmbush = SpellData::getSpellData(SpellID::IcyAmbush);
	icyAmbush.damageType = DamageType::Shadow;
	icyAmbush.skinNr = 1;
	icyAmbush.damage = 40;
	icyAmbush.damagePerSecond = 6;
	icyAmbush.cooldown = sf::seconds(8.f);
	icyAmbush.isBlocking = true;
	icyAmbush.isStunning = true;
	icyAmbush.duration = sf::seconds(2.f);
	icyAmbush.activeDuration = sf::seconds(10.f);
	icyAmbush.fightingTime = sf::seconds(1.f);
	icyAmbush.castingTime = sf::seconds(2.f);
	icyAmbush.fightAnimation = GameObjectState::Fighting3;
	icyAmbush.castingAnimation = GameObjectState::Casting3;
	icyAmbush.speed = 300.f;
	icyAmbush.range = 500.f;
	icyAmbush.isColliding = false;

	m_spellManager->addSpell(icyAmbush);

	m_spellManager->setCurrentSpell(0); // ultimative chop
}

void MorgianaBoss::onHit(Spell* spell) {
	if (m_isDead) return;
	if (!m_isBlocking) {
		Enemy::onHit(spell);
		return;
	}

	if (!spell->isAttachedToMob()) {
		// we'll send you right back!
		spell->setOwner(this);
		spell->reflect();
	}
}

void MorgianaBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	m_mainChar->setStunned(sf::seconds(4.f));

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *getBoundingBox();

	for (auto go : *m_screen->getObjects(_Enemy)) {
		if (auto* jeremy = dynamic_cast<JeremyBoss*>(go)) {
			rec.boundingBox.left += 20.f;
			jeremy->notifyMorgianaDeath(getPosition() + (m_level->collides(rec) ? sf::Vector2f() : sf::Vector2f(20.f, 0.f)));
			rec.boundingBox.left -= 20.f;
		}
		if (auto* roy = dynamic_cast<RoyBoss*>(go)) {
			rec.boundingBox.left -= 20.f;
			roy->notifyMorgianaDeath(getPosition() + (m_level->collides(rec) ? sf::Vector2f() : sf::Vector2f(-20.f, 0.f)));
			rec.boundingBox.left += 20.f;
		}
	}

	g_achievementManager->notifyAchievement(ACH_MERCENARY_ORDER, "morgiana");
}

void MorgianaBoss::notifyJeremyDeath(const sf::Vector2f& newPos) {
	// got your teleport!
	if (m_isDead) return;
	setPosition(newPos);
	setStunned(sf::seconds(5.f));
	m_scriptedBehavior->say("MorgianaJeremyDead", 5);
	m_isJeremyDead = true;
	m_attributes.currentHealthPoints = m_attributes.maxHealthPoints;
	m_attributes.calculateAttributes();
}

void MorgianaBoss::notifyRoyDeath(const sf::Vector2f& newPos) {
	if (m_isDead) return;
	// got your crit!
	setPosition(newPos);
	setStunned(sf::seconds(5.f));
	m_scriptedBehavior->say("MorgianaRoyDead", 5);
	m_attributes.critical = 100;
	m_attributes.currentHealthPoints = m_attributes.maxHealthPoints;
	m_attributes.calculateAttributes();
}

void MorgianaBoss::handleAttackInput() {
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 80.f) {
		m_spellManager->setCurrentSpell(0);
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
	else {
		if (m_isBlocking) return;
		int spell = m_isJeremyDead ? rand() % 2 + 1 : 1;
		m_spellManager->setCurrentSpell(spell);
		bool executed = m_spellManager->executeCurrentSpell(getCurrentTarget());
		if (spell == 1 && executed) {
			m_isBlocking = true;
			m_isInvincible = true;
			m_blockingTime = BLOCKING_TIME + sf::seconds(0.5f);
		}
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
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDropAlways(true);
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
	return "res/texture/bosses/spritesheet_boss_morgiana.png";
}

std::string MorgianaBoss::getDeathSoundPath() const {
	return "res/sound/mob/morgiana_death.ogg";
}
