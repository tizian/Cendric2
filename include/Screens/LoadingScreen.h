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

	void execOnEnter() override;
	void execOnExit() override;

private:
	void loadAsync() const;

	bool m_isRendered = false;
	sf::Sprite m_screenSprite;
	sf::RectangleShape m_blackRect;
	sf::Texture* m_texture = nullptr;

	WorldScreen* m_worldToLoad = nullptr;

	std::atomic<bool> m_threadDone;
	std::thread* m_thread;
	sf::Time m_timeToBlack;
};