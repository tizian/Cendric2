#include "Level/Enemies/SkeletonElementalEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton_Elemental, SkeletonElementalEnemy)

void SkeletonElementalEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 10 + 2;

	if (rand() % 100 > 80)
		loot.insert({ "fo_greaterhealingpotion", 1 });
}

void SkeletonElementalEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonElementalEnemy::SkeletonElementalEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonElementalEnemy::loadAttributes() {
	m_attributes.setHealth(150);
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.resistanceLight = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.critical = 10;
	m_attributes.calculateAttributes();
}

void SkeletonElementalEnemy::loadSpells() {
	SpellData spell;
	switch (m_skinNr) {
	case 0:
		spell = SpellData::getSpellData(SpellID::FireBall);
		spell.damage = 40;
		spell.damagePerSecond = 5;
		spell.duration = sf::seconds(2.f);
		spell.count = 2;
		spell.cooldown = sf::seconds(4);
		spell.fightingTime = sf::milliseconds(400);
		spell.castingTime = sf::milliseconds(500);
		spell.isBlocking = true;
		break;
	case 1:
		spell = SpellData::getSpellData(SpellID::IceBall);
		spell.damage = 40;
		spell.damagePerSecond = 5;
		spell.duration = sf::seconds(2.f);
		spell.count = 2;
		spell.cooldown = sf::seconds(4);
		spell.fightingTime = sf::milliseconds(400);
		spell.castingTime = sf::milliseconds(500);
		spell.isBlocking = true;
		break;
	case 2:
		spell = SpellData::getSpellData(SpellID::Projectile);
		spell.skinNr = 3;
		spell.damage = 40;
		spell.damagePerSecond = 5;
		spell.duration = sf::seconds(2.f);
		spell.count = 2;
		spell.cooldown = sf::seconds(4);
		spell.fightingTime = sf::milliseconds(400);
		spell.castingTime = sf::milliseconds(500);
		spell.isBlocking = true;
		break;
	case 3:
		spell = SpellData::getSpellData(SpellID::WindGust);
		spell.damage = 10;
		spell.damagePerSecond = 10;
		spell.duration = sf::seconds(4.f);
		spell.strength = 3;
		spell.cooldown = sf::seconds(4);
		spell.fightingTime = sf::milliseconds(400);
		spell.castingTime = sf::milliseconds(500);
		spell.range = 200.f;
		spell.boundingBox = sf::FloatRect(0.f, 0.f, spell.range, 60.f);
		break;
	}
	
	m_spellManager->addSpell(spell);

	m_spellManager->setCurrentSpell(0); 
}

void SkeletonElementalEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < ((m_skinNr == 3) ? 200.f : 500.f)) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

void SkeletonElementalEnemy::loadAnimation(int skinNr) {
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

	Animation* deadAnimation = new Animation(sf::milliseconds(70));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingStartAnimation = new Animation();
	fightingStartAnimation->setSpriteSheet(tex);
	fightingStartAnimation->addFrame(sf::IntRect(15 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(16 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(17 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(18 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, fightingStartAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(18 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->setLooped(false);
	
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// light
	LightData data(sf::Vector2f(40.f, -10.f), sf::Vector2f(70.f, 100.f), 0.8f);
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void SkeletonElementalEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	m_lightComponent->flipOffsetX(!m_enemyMovingBehavior->isFacingRight());
}

void SkeletonElementalEnemy::setDead() {
	Enemy::setDead();
	m_lightComponent->setVisible(false);
}

MovingBehavior* SkeletonElementalEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(20.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(150.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* SkeletonElementalEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonElementalEnemy::handleAttackInput, this));
	return behavior;
}

int SkeletonElementalEnemy::getMentalStrength() const {
	return 3;
}

std::string SkeletonElementalEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_skeletonelemental.png";
}

std::string SkeletonElementalEnemy::getDeathSoundPath() const {
	return "res/sound/mob/skeleton_death.ogg";
}