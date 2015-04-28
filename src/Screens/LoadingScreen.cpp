#include "Screens\LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(ScreenID screenToLoad, ResourceID resource)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_loading)));
	m_screenToLoad = screenToLoad;
	m_resource = resource;
}

Screen* LoadingScreen::update(sf::Time frameTime)
{
	switch (m_screenToLoad)
	{
	case ScreenID::Screen_game:
		return new GameScreen(m_resource);
	case ScreenID::Screen_map:
		return new MapScreen(m_resource);
	default:
		return this;
	}
}

void LoadingScreen::render(sf::RenderTarget &renderTarget)
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, static_cast<float>(WINDOW_HEIGHT) / (WINDOW_HEIGHT + BOTTOM_BORDER)));
	view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	renderTarget.setView(view);
	renderTarget.draw(m_screenSprite);
}

void LoadingScreen::execOnEnter(Screen *previousScreen)
{
}

void LoadingScreen::execOnExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_loading);
}