#pragma once

#include "global.h"

// some constants used by the GUI
class GUIConstants final {
private:
	GUIConstants() {}
public:
	static const float LEFT;
	static const float TOP;
	static const float LEFT_BAR;

	static const float GUI_WINDOW_HEIGHT;
	static const float GUI_TABS_TOP;

	static const float TEXT_OFFSET;
	static const float LABEL_OFFSET;
	static const float BUTTON_MARGIN;

	static const int CHARACTER_SIZE_S;
	static const int CHARACTER_SIZE_M;
	static const int CHARACTER_SIZE_L;
	static const int CHARACTER_SIZE_XL;
	static const int CHARACTER_SIZE_XXL;
	static const int CHARACTER_SIZE_XXXL;
	static const int CHARACTER_SIZE_DIALOGUE;

	// colors for windows
	static const sf::Color MAIN_COLOR;
	static const sf::Color BACK_COLOR;
	static const sf::Color ORNAMENT_COLOR;
};
