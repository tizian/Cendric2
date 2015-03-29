#include "stdafx.h"

SplashScreen::SplashScreen()
{
	cout << "Init SplashScreen...";
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_splashScreen)));
	cout << "...done." << endl;
}

Screen* SplashScreen::update(sf::Time frameTime)
{
	if (Keyboard::isKeyPressed(Keyboard::Unknown) || Mouse::isButtonPressed(Mouse::Button::Left))
	{
		cout << "(SplashScreen::update): Change Screen" << endl;
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
	cout << "Enter SplashScreen" << endl;
}

void SplashScreen::onExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_splashScreen);
	cout << "Exit SplashScreen" << endl;
}