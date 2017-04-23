#pragma once

#include <thread> 

#include "global.h"
#include "ResourceManager.h"
#include "Screens/MapScreen.h"
#include "Screens/LevelScreen.h"

class LoadingScreen final : public Screen {
public:
	LoadingScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	virtual void execOnEnter(const Screen *previousScreen) override;
	virtual void execOnExit(const Screen *nextScreen) override;

private:
	bool m_isRendered = false;
	BitmapText* m_title = nullptr;
	sf::Sprite m_screenSprite;

	LevelScreen* m_levelToLoad = nullptr;
	MapScreen* m_mapToLoad = nullptr;
};