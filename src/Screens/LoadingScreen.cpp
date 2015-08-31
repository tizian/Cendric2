#include "Screens/LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(CharacterCore* core) : Screen(core)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_loading)));
	if (core->getData().isInLevel)
	{
		m_levelToLoad = core->getData().currentLevel;
		m_mapToLoad = MapID::VOID;
	}
	else
	{
		m_levelToLoad = LevelID::VOID;
		m_mapToLoad = core->getData().currentMap;
	}
}

Screen* LoadingScreen::update(const sf::Time& frameTime)
{
	if (m_levelToLoad != LevelID::VOID)
	{
		return new LevelScreen(m_levelToLoad, getCharacterCore());
	}
	if (m_mapToLoad != MapID::VOID)
	{
		return new MapScreen(m_mapToLoad, getCharacterCore());
	}
	return this;
}

void LoadingScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
}

void LoadingScreen::execOnEnter(const Screen *previousScreen)
{
}

void LoadingScreen::execOnExit(const Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_loading);
}