#pragma once

#include <thread> 
#include <atomic>

#include "global.h"
#include "ResourceManager.h"
#include "Screens/MapScreen.h"
#include "Screens/LevelScreen.h"

class LoadingScreen final : public Screen {
public:
	LoadingScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(Screen *previousScreen) override;
	void execOnExit(Screen *nextScreen) override;

private:
	void loadAsync() const;

	bool m_isRendered = false;
	sf::Sprite m_screenSprite;
	sf::RectangleShape m_blackRect;
	sf::Texture m_texture;

	LevelScreen* m_levelToLoad = nullptr;
	MapScreen* m_mapToLoad = nullptr;

	std::atomic<bool> m_threadDone;
	std::thread* m_thread;
	sf::Time m_timeToBlack;
};