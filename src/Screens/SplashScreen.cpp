#include "Screens\SplashScreen.h"

using namespace std;

SplashScreen::SplashScreen()
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_splash)));
}

void SplashScreen::execOnEnter(Screen* previousScreen)
{
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 140.f));
	fireBasket2->setPosition(sf::Vector2f(998.f, 140.f));
	addObject(GameObjectType::_Undefined, fireBasket1);
	addObject(GameObjectType::_Undefined, fireBasket2);
}

Screen* SplashScreen::update(sf::Time frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || g_inputController->isMouseJustPressedLeft())
	{
		return new MenuScreen();
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
	return this;
}

void SplashScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
}

void SplashScreen::execOnExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash);
}