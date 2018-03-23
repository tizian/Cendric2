#include "Level/Enemies/WolfBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/WolfBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Wolf, WolfBoss)

const std::string WolfBoss::TRANSFORM_SPRITEPATH = "res/texture/cendric/cendric_transform_wolf.png";

float WolfBoss::getConfiguredDistanceToHPBar() const {
	return 80.f;
}

WolfBoss::WolfBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
	
	m_isInvincible = true;
}

void WolfBoss::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistancePhysical = 100;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void WolfBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 70;
	chopSpell.activeDuration = sf::seconds(1000.f);
	chopSpell.cooldown = sf::seconds(0.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 120, 140);
	chopSpell.spellOffset = sf::Vector2f(-20.f, -50.f);
	chopSpell.fightingTime = sf::seconds(1000.f);
	chopSpell.castingTime = sf::seconds(1.f);
	chopSpell.castingAnimation = GameObjectState::Casting;
	chopSpell.fightAnimation = GameObjectState::Walking;

	m_spellManager->addSpell(chopSpell);

	// transform spell sprite for cendric
	g_resourceManager->loadTexture(TRANSFORM_SPRITEPATH, ResourceType::Level);
	Animation* transformedAnimation = new Animation();
	transformedAnimation->setSpriteSheet(g_resourceManager->getTexture(TRANSFORM_SPRITEPATH));
	transformedAnimation->addFrame(sf::IntRect(0, 0, 110, 120));
	m_mainChar->addAnimation(GameObjectState::Broken, transformedAnimation);

	SpellData transformBeamSpell = SpellData::getSpellData(SpellID::WindGust);
	transformBeamSpell.id = SpellID::TransformBeam;
	transformBeamSpell.activeDuration = sf::seconds(2.5f);
	transformBeamSpell.damagePerSecond = 0;
	transformBeamSpell.damageType = DamageType::VOID;
	transformBeamSpell.cooldown = sf::seconds(10.f);
	transformBeamSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);
	transformBeamSpell.spellOffset = sf::Vector2f(12.f, -120.f);
	transformBeamSpell.fightingTime = sf::seconds(3.f);
	transformBeamSpell.castingTime = sf::seconds(2.f);
	transformBeamSpell.castingAnimation = GameObjectState::Casting2;
	transformBeamSpell.fightAnimation = GameObjectState::Fighting2;
	transformBeamSpell.spellSoundPaths.clear();
	transformBeamSpell.creatorSoundPaths = { "res/sound/spell/transformbeam.ogg" };

	m_spellManager->addSpell(transformBeamSpell);

	SpellData windgustSpell = SpellData::getSpellData(SpellID::WindGust);
	windgustSpell.activeDuration = sf::seconds(1000.f);
	windgustSpell.damagePerSecond = 10;
	windgustSpell.damageType = DamageType::Ice;
	windgustSpell.cooldown = sf::seconds(0.f);
	windgustSpell.boundingBox = sf::FloatRect(0, 0, 1200, 350);
	windgustSpell.spellOffset = sf::Vector2f(10.f, -250.f);
	windgustSpell.fightingTime = sf::seconds(1000.f);
	windgustSpell.castingTime = sf::seconds(1.f);
	windgustSpell.castingAnimation = GameObjectState::Casting3;
	windgustSpell.fightAnimation = GameObjectState::Fighting3;
	windgustSpell.strength = 3;

	m_spellManager->addSpell(windgustSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void WolfBoss::handleAttackInput() {
	if (getState() == GameObjectState::Fighting3 && std::abs(m_mainChar->getPosition().y - getPosition().y) < 100.f) {
		// cendric has come down.
		setReady();
		clearSpells(true);
		return;
	}

	// We are doing something else
	if (getState() != GameObjectState::Idle)
		return;

	// Cendric is standing too high to reach with charge or beam, so we're going to use windgust
	if (std::abs(m_mainChar->getPosition().y - getPosition().y) > 100.f)
		m_spellManager->setCurrentSpell(2); // windgust
	else {
		if (std::abs(m_mainChar->getPosition().x - getPosition().x) > 1000.f) {
			m_spellManager->setCurrentSpell(0); // only charge
		}
		if (m_mainChar->isStunned()) {
			m_spellManager->getSpell(0)->damage = 10000; // ten thousand!
			m_spellManager->setCurrentSpell(0); // only charge
		}
		else {
			m_spellManager->setCurrentSpell(rand() % 2); // charge or beam
		}
	}
	
	m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
}

void WolfBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 200.f, 90.f));
	setSpriteOffset(sf::Vector2f(-50.f, -160.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.05f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 13; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 300, 0, 300, 250));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(4 * 300, 250, 300, 250));
	idleAnimation->addFrame(sf::IntRect(5 * 300, 250, 300, 250));
	idleAnimation->addFrame(sf::IntRect(6 * 300, 250, 300, 250));
	idleAnimation->addFrame(sf::IntRect(5 * 300, 250, 300, 250));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(3 * 300, 250, 300, 250));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(9 * 300, 750, 300, 250));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// casting before charge attack
	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->addFrame(sf::IntRect(0 * 300, 250, 300, 250));
	castingAnimation->addFrame(sf::IntRect(1 * 300, 250, 300, 250));
	castingAnimation->addFrame(sf::IntRect(2 * 300, 250, 300, 250));
	castingAnimation->addFrame(sf::IntRect(1 * 300, 250, 300, 250));

	addAnimation(GameObjectState::Casting, castingAnimation);

	// casting before beam attack
	Animation* casting2Animation = new Animation();
	casting2Animation->setSpriteSheet(tex);
	for (int i = 0; i < 12; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * 300, 500, 300, 250));
	}
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// beam attack
	Animation* beamAnimation = new Animation();
	beamAnimation->setSpriteSheet(tex);
	beamAnimation->addFrame(sf::IntRect(11 * 300, 500, 300, 250));

	addAnimation(GameObjectState::Fighting2, beamAnimation);

	// casting before windgust attack
	Animation* casting3Animation = new Animation();
	casting3Animation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		casting3Animation->addFrame(sf::IntRect(i * 300, 500, 300, 250));
	}
	casting3Animation->setLooped(false);

	addAnimation(GameObjectState::Casting3, casting3Animation);

	// windgust attack
	Animation* windgustAnimation = new Animation();
	windgustAnimation->setSpriteSheet(tex);
	windgustAnimation->addFrame(sf::IntRect(7 * 300, 250, 300, 250));

	addAnimation(GameObjectState::Fighting3, windgustAnimation);

	// trip over
	Animation* tripoverAnimation = new Animation();
	tripoverAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 5; ++i) {
		tripoverAnimation->addFrame(sf::IntRect(i * 300, 750, 300, 250));
	}
	tripoverAnimation->setLooped(false);

	addAnimation(GameObjectState::TripOver, tripoverAnimation);

	// laying
	Animation* layingAnimation = new Animation();
	layingAnimation->setSpriteSheet(tex);
	layingAnimation->addFrame(sf::IntRect(4 * 300, 750, 300, 250));
	layingAnimation->addFrame(sf::IntRect(5 * 300, 750, 300, 250));
	layingAnimation->addFrame(sf::IntRect(6 * 300, 750, 300, 250));
	layingAnimation->addFrame(sf::IntRect(5 * 300, 750, 300, 250));

	addAnimation(GameObjectState::Laying, layingAnimation);

	// standup
	Animation* standupAnimation = new Animation();
	standupAnimation->setSpriteSheet(tex);
	for (int i = 8; i < 11; ++i) {
		standupAnimation->addFrame(sf::IntRect(i * 300, 750, 300, 250));
	}
	standupAnimation->setLooped(false);

	addAnimation(GameObjectState::Standup, standupAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticles();
}

MovingBehavior* WolfBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new WolfBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(10000.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(300.f);
	behavior->setMaxVelocityX(700.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* WolfBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&WolfBoss::handleAttackInput, this));
	return behavior;
}

sf::Time WolfBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

std::string WolfBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_wolfmonster.png";
}
