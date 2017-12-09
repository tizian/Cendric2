#include "Level/Enemies/VeliusBossClone.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Screens/LevelScreen.h"
#include "World/CustomParticleUpdaters.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::VeliusClone, VeliusBossClone)

VeliusBossClone::VeliusBossClone(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void VeliusBossClone::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) return;

	
}

void VeliusBossClone::setDead() {
	Enemy::setDead();
	setState(GameObjectState::Dead, true);

	m_isLooted = true;
	m_interactComponent->setInteractable(false);
}

void VeliusBossClone::loadAttributes() {
	m_attributes.setHealth(1);
	m_attributes.calculateAttributes();
}

void VeliusBossClone::loadSpells() {

}

void VeliusBossClone::handleAttackInput() {
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
	deadAnimation->addFrame(sf::IntRect(10 * width, height, width, height));
	deadAnimation->addFrame(sf::IntRect(11 * width, height, width, height));
	deadAnimation->addFrame(sf::IntRect(12 * width, height, width, height));
	deadAnimation->addFrame(sf::IntRect(13 * width, height, width, height));
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
	return nullptr;
}

AttackingBehavior* VeliusBossClone::createAttackingBehavior(bool asAlly) {
	return nullptr;
}

std::string VeliusBossClone::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_velius.png";
}

std::string VeliusBossClone::getDeathSoundPath() const {
	return "res/sound/mob/yashaadd_death.ogg";
}

