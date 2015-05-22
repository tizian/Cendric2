#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screens/MapScreen.h"
#include "Screens/GameScreen.h"

#include "Enums/MapID.h"
#include "Enums/LevelID.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen(LevelID level, CharacterCore* core);
	LoadingScreen(MapID map, CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	virtual void execOnEnter(const Screen *previousScreen) override;
	virtual void execOnExit(const Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;

	LevelID m_levelToLoad;
	MapID m_mapToLoad;
};