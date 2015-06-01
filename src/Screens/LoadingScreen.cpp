#include "Screens/LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(LevelID id, CharacterCore* core) : Screen(core)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_loading)));
	m_levelToLoad = id;
	m_mapToLoad = MapID::Void;
}

LoadingScreen::LoadingScreen(MapID id, CharacterCore* core) : Screen(core)
{
	m_screenSprite = sf::Sprite(*g_resourceManager->getTexture(ResourceID::Texture_screen_loading));
	m_mapToLoad = id;
	m_levelToLoad = LevelID::Void;
}

Screen* LoadingScreen::update(const sf::Time& frameTime)
{
	if (m_levelToLoad != LevelID::Void)
	{
		return new GameScreen(m_levelToLoad, getCharacterCore());
	}
	if (m_mapToLoad != MapID::Void)
	{
		return new MapScreen(m_mapToLoad, getCharacterCore());
	}
	return this;
}

void LoadingScreen::render(sf::RenderTarget &renderTarget)
{
	setViewToStandardView(renderTarget);
	renderTarget.draw(m_screenSprite);
}

void LoadingScreen::execOnEnter(const Screen *previousScreen)
{
}

void LoadingScreen::execOnExit(const Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_loading);
}