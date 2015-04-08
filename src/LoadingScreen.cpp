#include "LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(ScreenID screenToLoad, ResourceID resource)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_loading)));
	m_screenToLoad = screenToLoad;
	m_resource = resource;
	m_isLoading = false;
}

Screen* LoadingScreen::update(sf::Time frameTime)
{
	if (m_isLoading) 
	{
		switch (m_screenToLoad)
		{
		case ScreenID::Screen_game:
			return new GameScreen(m_resource);
		case ScreenID::Screen_map:
			return new MapScreen(m_resource);
		default:
			break;
		}
	}
	m_isLoading = true;
	return this;
}

void LoadingScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.draw(m_screenSprite);
}

void LoadingScreen::onEnter(Screen *previousScreen)
{
}

void LoadingScreen::onExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_loading);
}