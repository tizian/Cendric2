#include "Screens\MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen()
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

Screen* MenuScreen::update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return new LoadingScreen(ScreenID::Screen_map, ResourceID::Map_testmap);
	}
	return this;
}

void MenuScreen::render(sf::RenderTarget &renderTarget) 
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
}

void MenuScreen::execOnEnter(Screen *previousScreen)
{
}

void MenuScreen::execOnExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
}