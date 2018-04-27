#pragma once

#include "global.h"
#include "Screen.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"

class KeyBindingsScreen final : public Screen {
public:
	KeyBindingsScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter() override;
	void execOnExit() override;

private:
	void onBack();
	void onKeyboardKeyBindings();
	void onGamepadKeyBindings();

private:
	BitmapText* m_title = nullptr;
};