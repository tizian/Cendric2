#include "Level/Enemies/VeliusBossClone.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/NeutralWalkingBehavior.h"
#include "Screens/LevelScreen.h"
#include "World/CustomParticleUpdaters.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::VeliusClone, VeliusBossClone)

VeliusBossClone::VeliusBossClone(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void VeliusBossClone::setDead() {
	Enemy::setDead();
	setState(GameObjectState::Dead, true);

	m_isLooted = true;
	m_interactComponent->setInteractable(false);
}

void VeliusBossClone::onHit(Spell* spell) {
	m_spellManager->setCurrentSpell(0);
	m_spellManager->executeCurrentSpell(m_mainChar);

	Enemy::onHit(spell);
}

void VeliusBossClone::loadAttributes() {
	m_attributes.setHealth(1);
	m_attributes.calculateAttributes();
}

void VeliusBossClone::loadSpells() {
	// the pretty brutal spell that gets cast when a clone is shattered
	SpellData projectile = SpellData::getSpellData(SpellID::TargetingProjectile);
	projectile.cooldown = sf::seconds(0);
	projectile.activeDuration = sf::seconds(10.f);
	projectile.damageType = DamageType::Shadow;
	projectile.damage = 40;
	projectile.damagePerSecond = 5;
	projectile.speed = 400;
	projectile.count = 3;
	projectile.skinNr = 1;
	projectile.strength = 4;
	projectile.isColliding = false;
	projectile.isStunning = true;
	projectile.duration = sf::seconds(2.f);
	projectile.fightingTime = sf::seconds(0.f);
	projectile.castingTime = sf::seconds(0.f);

	m_spellManager->addSpell(projectile);

	// the "default" spells

	// illusion
	SpellData spell = SpellData::getSpellData(SpellID::FireBall);
	spell.skinNr = 6;
	spell.damage = 30;
	spell.count = 1;
	spell.damageType = DamageType::Shadow;
	spell.duration = sf::seconds(1.f);
	spell.cooldown = sf::seconds(5.f);
	spell.isBlocking = true;
	spell.fightingTime = sf::seconds(0.f);
	spell.castingTime = sf::seconds(0.45f);
	spell.castingAnimation = GameObjectState::Casting;
	spell.speed = 300;
	spell.spellOffset = sf::Vector2f(10.f, 0.f);

	m_spellManager->addSpell(spell);
	m_spellManager->setInitialCooldown(sf::seconds(randomFloat(0.f, 5.f)), SpellID::FireBall);

	spell.skinNr = 7;
	spell.damageType = DamageType::Ice;
	spell.isStunning = true;
	spell.duration = sf::seconds(1.f);

	m_spellManager->addSpell(spell);
}

void VeliusBossClone::handleAttackInput() {
	m_spellManager->setCurrentSpell(m_skinNr + 1); 
	m_spellManager->executeCurrentSpell(m_mainChar);
}

void VeliusBossClone::loadAnimation(int skinNr) {
	int width = 120;
	int height = 150;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 115.f));
	setSpriteOffset(sf::Vector2f(-50.f, -35.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, height, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 14; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}
	deadAnimation->addFrame(sf::IntRect(17 * width, height, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// the "throwning" spell
	Animation* castingAnimation = new Animation(sf::seconds(0.15f));
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->addFrame(sf::IntRect(14 * width, height, width, height));
	castingAnimation->addFrame(sf::IntRect(15 * width, height, width, height));
	castingAnimation->addFrame(sf::IntRect(16 * width, height, width, height));
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	// component: light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, 0.f),
		sf::Vector2f(100.f, 100.f), 0.6f), this));
}

MovingBehavior* VeliusBossClone::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new NeutralWalkingBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYUp(0.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(0.f);
	behavior->calculateJumpHeight();
	return  behavior;
}

AttackingBehavior* VeliusBossClone::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&VeliusBossClone::handleAttackInput, this));
	return behavior;
}

std::string VeliusBossClone::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_velius.png";
}

std::string VeliusBossClone::getDeathSoundPath() const {
	return "res/sound/mob/yashaadd_death.ogg";
}

