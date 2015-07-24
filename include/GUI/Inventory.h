#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"

// the inventory, as displayed in a level or a map
// it takes its information directly from the character core
class Inventory
{
public:
	Inventory(CharacterCore* core);
	~Inventory();

	void show();
	void hide();

	void render(sf::RenderTarget& target) const;
	void update();

private:
	CharacterCore* m_core;
	bool m_isVisible = false;

	// reloads the inventory text & items, depending on the core
	void reload();

	Window* m_window;
	BitmapText m_inventoryText;

	const float TEXT_OFFSET = 20.f;
	const float INVENTORY_TOP = 50.f;
	const float INVENTORY_LEFT = 50.f;
	const float INVENTORY_HEIGHT = 500.f;
	const float INVENTORY_WIDTH = 700.f;
};