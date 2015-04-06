#include "Screens\MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen()
{
	cout << "Init MenuScreen...";
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_menuScreen)));
	cout << "...done." << endl;
}

Screen* MenuScreen::update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return new MapScreen();
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