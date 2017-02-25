#include "Level/Enemies/JanusBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Janus, JanusBoss)

float JanusBoss::getConfiguredDistanceToHPBar() const {
	return 50.f;
}

JanusBoss::JanusBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
	
	m_isAlwaysUpdate = true;
}

void JanusBoss::loadAttributes() {
	m_attributes.setHealth(800);
	m_attributes.resistanceIce = 200;
	m_attributes.resistanceFire = 50;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void JanusBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 80;
	chopSpell.activeDuration = sf::seconds(1.5f);
	chopSpell.cooldown = sf::seconds(4.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 80, 50);
	chopSpell.spellOffset = sf::Vector2f(-40.f, 0.f);
	chopSpell.fightingTime = sf::seconds(0);

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop

	m_spellManager->setGlobalCooldown(sf::seconds(2.f));
}

void JanusBoss::handleAttackInput() {
	m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
}

void JanusBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 135.f));
	setSpriteOffset(sf::Vector2f(-70.f, -65.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 200;
	int height = 200;

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(0, 0, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// nose dive
	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(6 * width, 0, width, height));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// casting before thunder attack
	Animation* casting3Animation = new Animation();
	casting3Animation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		casting3Animation->addFrame(sf::IntRect(i * width, height, width, height));
	}

	addAnimation(GameObjectState::Casting3, casting3Animation);

	// casting before projectile attack
	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 2 * height, width, height));
	}
	for (int i = 0; i < 6; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 3 * height, width, height));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	// casting before stunning projectile attack
	Animation* casting2Animation = new Animation();
	casting2Animation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * width, 4 * height, width, height));
	}

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticleSystem();

	LightData data(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 150.f, 0.5f);
	addComponent(new LightComponent(data, this));
}

MovingBehavior* JanusBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AggressiveWalkingBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(200.f);
	behavior->setMaxVelocityX(200.f);
	return behavior;
}

AttackingBehavior* JanusBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&JanusBoss::handleAttackInput, this));
	return behavior;
}

sf::Time JanusBoss::getConfiguredWaitingTime() const {
	return sf::seconds(0);
}

std::string JanusBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_janus.png";
}