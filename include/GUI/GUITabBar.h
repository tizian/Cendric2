#pragma once

#include "global.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/GUITabButton.h"

class WorldInterface;

class GUITabBar {
public:
	GUITabBar(WorldInterface* _interface, int size);
	~GUITabBar();

	void show(int index);
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void setPosition(const sf::Vector2f& pos);
	void setButtonTexture(int index, const sf::Texture* tex, int x);
	void setButtonText(int index, const std::string& text);

	int getActiveElement() const;

private:
	WorldInterface* getInterface() const;
	void init();

private:
	bool m_isVisible = false;
	int m_size = 0;

	WorldInterface* m_worldInterface;
	std::vector<GUITabButton*> m_buttons;
	sf::Vector2f m_position;

	Window* m_window = nullptr;

public:
	static const int WIDTH;
	static const int BUTTON_MARGIN;
};