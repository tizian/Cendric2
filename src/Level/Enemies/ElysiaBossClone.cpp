#include "Level/Enemies/ElysiaBossClone.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Elysia_Clone, ElysiaBossClone)

static const std::string SCREECH_PATH = "res/sound/mob/elysia_screech.ogg";


ElysiaBossClone::ElysiaBossClone(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isInvincible = true;
	m_isAlwaysUpdate = true;

	// let's stun that boy who touched my precious egg...
	g_resourceManager->loadSoundbuffer(SCREECH_PATH, ResourceType::Unique, this);
	g_resourceManager->playSound(SCREECH_PATH);
	m_mainChar->setStunned(sf::seconds(1000));
	hasReachedMainchar = false;
}

ElysiaBossClone::~ElysiaBossClone() {
	g_resourceManager->deleteUniqueResources(this);
}

void ElysiaBossClone::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
}

void ElysiaBossClone::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
}

float ElysiaBossClone::getConfiguredDistanceToHPBar() const {
	return 50.f;
}

void ElysiaBossClone::update(const sf::Time& frameTime) {
	if (!hasReachedMainchar) {
		m_animatedSprite.setFlippedX(false);
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));

		if (dist(m_mainChar->getCenter(), getCenter()) < 150.f) {
			hasReachedMainchar = true;
			setCurrentAnimation(getAnimation(GameObjectState::Flying));
			setSpriteRotation(0.f);

			getMovingBehavior()->setMaxVelocityYDown(300.f);
			getMovingBehavior()->setMaxVelocityYUp(150.f);
			getMovingBehavior()->setMaxVelocityX(150.f);

			m_mainChar->getMovingBehavior()->setEnabled(false);
		}
	}
	else {
		m_mainChar->setPosition(getPosition() + sf::Vector2f(0.f, 50.f));
	}

	Enemy::update(frameTime);
}

void ElysiaBossClone::loadAttributes() {
	m_attributes.setHealth(1000);
	m_attributes.calculateAttributes();
}

void ElysiaBossClone::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 0;
	chopSpell.activeDuration = sf::seconds(0.1f);
	chopSpell.cooldown = sf::seconds(0.1f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 180, 150);
	chopSpell.spellOffset = sf::Vector2f(-90.f, 0.f);
	chopSpell.fightingTime = sf::seconds(0.2f);
	chopSpell.fightAnimation = GameObjectState::Fighting;

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void ElysiaBossClone::handleAttackInput() {
	if (hasReachedMainchar) return;

	m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
}

void ElysiaBossClone::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 80.f, 50.f));
	setSpriteOffset(sf::Vector2f(-50.f, -50.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 180;
	int height = 150;

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// nose dive
	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(6 * width, 0, width, height));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	LightData data(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 150.f, 0.5f);
	addComponent(new LightComponent(data, this));
}

MovingBehavior* ElysiaBossClone::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;

	behavior = new AggressiveFlyingBehavior(this);
	behavior->setCollisionsEnabled(false);
	behavior->setApproachingDistance(0.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(300.f);
	behavior->setMaxVelocityX(300.f);
	return behavior;
}

AttackingBehavior* ElysiaBossClone::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&ElysiaBossClone::handleAttackInput, this));
	return behavior;
}

int ElysiaBossClone::getMentalStrength() const {
	return 4;
}

sf::Time ElysiaBossClone::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

std::string ElysiaBossClone::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_elysia.png";
}
