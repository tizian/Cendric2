#include "stdafx.h"

MenuScreen::MenuScreen()
{
	cout << "Init MenuScreen...";
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_menuScreen)));
	cout << "...done." << endl;
}

Screen* MenuScreen::update(sf::Time frameTime)
{
	if (Keyboard::isKeyPressed(Keyboard::Return) || Mouse::isButtonPressed(Mouse::Button::Left))
	{
		return new GameScreen();
	}
	return this;
}

void MenuScreen::render(sf::RenderTarget &renderTarget) 
{
	renderTarget.draw(m_screenSprite);
}

void MenuScreen::onEnter(Screen *previousScreen)
{
	cout << "Enter MenuScreen" << endl;
}

void MenuScreen::onExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_menuScreen);
	cout << "Exit MenuScreen" << endl;
}