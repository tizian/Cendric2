#include "Level/Enemies/WispEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"

REGISTER_ENEMY(EnemyID::Wisp, WispEnemy)

void WispEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 10; 
}

void WispEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 5;
}

WispEnemy::WispEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void WispEnemy::loadAttributes() {
	m_attributes.setHealth(5);
	m_attributes.resistanceLight = 10000;

	if (m_skinNr == 0) {
		m_attributes.resistanceShadow = 10000;
	}
	else if (m_skinNr == 1) {
		m_attributes.resistanceFire = 10000;
	}
	
	m_attributes.calculateAttributes();
}

void WispEnemy::loadSpells() {
	SpellData shadowAureola = SpellData::getSpellData(SpellID::Aureola);
	shadowAureola.skinNr = m_skinNr + 1;
	shadowAureola.damage = 5;
	shadowAureola.isStunning = false;
	shadowAureola.damagePerSecond = m_skinNr + 5;
	shadowAureola.duration = sf::seconds(3.f);
	shadowAureola.cooldown = sf::seconds(5.f);
	shadowAureola.damageType = m_skinNr == 1 ? DamageType::Fire : DamageType::Shadow;
	shadowAureola.range = 200.f;
	shadowAureola.speed = 150.f;
	shadowAureola.fightingTime = sf::milliseconds(8 * 100);
	shadowAureola.castingAnimation = GameObjectState::Fighting;
	shadowAureola.castingTime = sf::milliseconds(8 * 100);

	m_spellManager->addSpell(shadowAureola);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* WispEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(70.f);
	behavior->setMaxVelocityYUp(70.f);
	behavior->setMaxVelocityX(70.f);
	return behavior;
}

AttackingBehavior* WispEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(400.f);
	behavior->setAttackInput(std::bind(&WispEnemy::handleAttackInput, this));
	return behavior;
}

void WispEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 200.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		}
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
}

sf::Time WispEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 1));
}

sf::Time WispEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void WispEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 20.f, 20.f));
	setSpriteOffset(sf::Vector2f(-30.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240, 80, 80));
	}
	for (int i = 3; i > -1; --i) {
		flyingAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240, 80, 80));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240, 80, 80));
	}
	for (int i = 3; i > -1; --i) {
		idleAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240, 80, 80));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240 + 80, 80, 80));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation(sf::seconds(0.2f));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 2; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * 80, skinNr * 240 + 160, 80, 80));
	}

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	LightData data(sf::Vector2f(10.f, 10.f), 100.f, 0.5f);
	addComponent(new LightComponent(data, this));
}

std::string WispEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_wisp.png";
}

std::string WispEnemy::getDeathSoundPath() const {
	return "res/sound/mob/wisp_death.ogg";
}