#include "Misc/FireBasket.h"

FireBasket::FireBasket() : AnimatedGameObject() {
	load();
}

void FireBasket::load() {
	setBoundingBox(sf::FloatRect(0, 0, 192, 800));

	Animation idleAnimation(sf::seconds(0.08f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_screen_splash_fireanimation));
	for (int i = 0; i < 38; i++) {
		idleAnimation.addFrame(sf::IntRect(192 * i, 0, 192, 800));
	}
	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

GameObjectType FireBasket::getConfiguredType() const {
	return GameObjectType::_Undefined;
}