#include "Level/Enemies/VeliusVictim.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "Level/MOBBehavior/MovingBehaviors/VeliusVictimMovingBehavior.h"

REGISTER_ENEMY(EnemyID::VeliusVictim, VeliusVictim)

VeliusVictim::VeliusVictim(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	m_isAlwaysUpdate = true;
}

void VeliusVictim::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) return;
}

void VeliusVictim::setDead() {
	Enemy::setDead();
}

void VeliusVictim::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.calculateAttributes();
}

void VeliusVictim::handleAttackInput() {}

void VeliusVictim::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	int width = 120;
	int height = 170;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, skinNr * height, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* kneelingAnimation = new Animation();
	kneelingAnimation->setSpriteSheet(tex);
	kneelingAnimation->addFrame(sf::IntRect(10 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Inactive, kneelingAnimation);

	Animation* layingAnimation = new Animation();
	layingAnimation->setSpriteSheet(tex);
	layingAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Laying, layingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Inactive);
	playCurrentAnimation(true);
}

MovingBehavior* VeliusVictim::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior = new VeliusVictimMovingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* VeliusVictim::createAttackingBehavior(bool asAlly) {
	return nullptr;
}

std::string VeliusVictim::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_veliusvictim.png";
}

std::string VeliusVictim::getDeathSoundPath() const {
	switch(m_skinNr)
	{
	case 2:
		return "res/sound/mob/morgiana_death.ogg";
	case 1:
		return "res/sound/mob/jeremy_death.ogg";
	default:
		return "res/sound/mob/roy_death.ogg";
	}
}

