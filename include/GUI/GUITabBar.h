#pragma once

#include "global.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"

class LevelInterface;
class MapInterface;
class WorldInterface;
class GUITabButton;

class GUITabBar {
public:
	GUITabBar(LevelInterface* _interface);
	GUITabBar(MapInterface* _interface);
	~GUITabBar();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

private:
	WorldInterface* getInterface() const;
	void init();

private:
	bool m_isVisible = false;

	LevelInterface* m_levelInterface = nullptr;
	MapInterface* m_mapInterface = nullptr;

	std::vector<GUITabButton*> m_buttons;
	
	sf::Vector2f m_position;

	Window* m_window = nullptr;

	float LEFT;
	float TOP;
	float WIDTH;
	float HEIGHT;
};