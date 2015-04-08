#include "Screens\SplashScreen.h"

using namespace std;

SplashScreen::SplashScreen()
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_splash)));
}

Screen* SplashScreen::update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return new MenuScreen();
	}
	return this;
}

void SplashScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.draw(m_screenSprite);
}

void SplashScreen::onEnter(Screen *previousScreen)
{
}

void SplashScreen::onExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash);
}