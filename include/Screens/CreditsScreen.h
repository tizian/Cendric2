#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"

class CreditsScreen : public Screen {
public:
	CreditsScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;
	Button* m_backButton = nullptr;
	BitmapText* m_title = nullptr;
	BitmapText* m_credits = nullptr;
};