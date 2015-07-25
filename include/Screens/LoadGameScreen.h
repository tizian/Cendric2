#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "GUI/SaveGameWindow.h"
#include "GUI/YesOrNoForm.h"

class LoadGameScreen : public Screen
{
public:
	LoadGameScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	Button* m_backButton = nullptr;
	Button* m_loadButton = nullptr;
	BitmapText* m_title = nullptr;
	SaveGameWindow* m_saveGameWindow = nullptr;

	YesOrNoForm* m_yesOrNoForm = nullptr;
	CharacterCore* m_newCharacterCore = nullptr;
};