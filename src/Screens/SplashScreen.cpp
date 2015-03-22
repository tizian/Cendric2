#include "stdafx.h"

SplashScreen::SplashScreen()
{
	cout << "Init SplashScreen...";
	if (!m_screenTexture.loadFromFile("res/screens/screen_splash.png"))
	{
		printf("SplashScreen: Error at opening file res/screens/screen_splash.png \n");
		// TODO: what to do if resource loading fails in constructor?
	}
	m_screenSprite = sf::Sprite(m_screenTexture);
	cout << "...done." << endl;
}

Screen* SplashScreen::update(float dt)
{
	// TODO: not sure if it is a good idea to poll events separately for every Screen...
	if (Keyboard::isKeyPressed(Keyboard::Unknown) || Mouse::isButtonPressed(Mouse::Button::Left))
	{
		cout << "(SplashScreen::update): Change Screen" << endl;
		return new MenuScreen();
	}
	return this;
}

void SplashScreen::render(sf::RenderTarget &renderTarget) const
{
	renderTarget.draw(m_screenSprite);
}

void SplashScreen::onEnter(Screen *previousScreen)
{
	cout << "Enter SplashScreen" << endl;
}

void SplashScreen::onExit(Screen *nextScreen)
{
	cout << "Exit SplashScreen" << endl;
}