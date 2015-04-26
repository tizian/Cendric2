#include "FireBasket.h"

FireBasket::FireBasket()
{
	load();
}

void FireBasket::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0, 0, 192, 600));

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_screen_splash_fireanimation));
	idleAnimation.addFrame(sf::IntRect(0, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 2, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 3, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 4, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 5, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 6, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 7, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 8, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 9, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 10, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 11, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 12, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 13, 0, 192, 600));
	idleAnimation.addFrame(sf::IntRect(192 * 14, 0, 192, 600));

	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

GameObjectType FireBasket::getConfiguredType() const
{
	return GameObjectType::_Undefined;
}