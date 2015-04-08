#pragma once

#include "global.h"
#include "Map.h"
#include "MapMainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "LoadingScreen.h"

class MapScreen : public Screen
{
public:
	MapScreen(ResourceID map);
	~MapScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

private:
	Map m_currentMap;
	MapMainCharacter* m_mainChar;
};