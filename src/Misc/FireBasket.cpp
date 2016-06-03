#include "Misc/FireBasket.h"
#include "GlobalResource.h"

const std::string FIREANIMATION_PATH = "res/assets/misc/spritesheet_fireanimation.png";

FireBasket::FireBasket() : AnimatedGameObject() {
	load();
}

void FireBasket::load() {
	g_resourceManager->loadTexture(FIREANIMATION_PATH, ResourceType::Global);
	setBoundingBox(sf::FloatRect(0, 0, 192, 800));

	Animation* idleAnimation = new Animation(sf::seconds(0.08f));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(FIREANIMATION_PATH));
	for (int i = 0; i < 38; i++) {
		idleAnimation->addFrame(sf::IntRect(192 * i, 0, 192, 800));
	}
	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	m_animatedSprite.setScale(0.86f, 0.86f);
}

GameObjectType FireBasket::getConfiguredType() const {
	return GameObjectType::_Undefined;
}