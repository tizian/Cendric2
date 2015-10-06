#include "Misc/FireBasket.h"

FireBasket::FireBasket() : GameObject() {
	load();
}

void FireBasket::load() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0, 0, 192, 600));

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_screen_splash_fireanimation));
	for (int i = 0; i < 16; i++) {
		idleAnimation.addFrame(sf::IntRect(192 * i, 0, 192, 600));
	}
	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

GameObjectType FireBasket::getConfiguredType() const {
	return GameObjectType::_Undefined;
}