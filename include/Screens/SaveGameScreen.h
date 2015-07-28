#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "GUI/SaveGameWindow.h"
#include "GUI/YesOrNoForm.h"
#include "GUI/NewSaveGameForm.h"

class SaveGameScreen : public Screen
{
public:
	SaveGameScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	void setAllButtonsEnabled(bool value) override;

	Button* m_backButton = nullptr;
	Button* m_saveButton = nullptr;
	Button* m_newSaveGameButton = nullptr;
	Button* m_deleteSaveGameButton = nullptr;
	BitmapText* m_title = nullptr;

	SaveGameWindow* m_saveGameWindow = nullptr;
	NewSaveGameForm* m_newSaveGameForm = nullptr;
	YesOrNoForm* m_yesOrNoForm = nullptr;

	// pretty little agents to give to our yes or no forms.
	void onNoPressed();
	void onOverwriteSaveGamePressed();
	void onDeleteSaveGamePressed();

	// pretty little agents to give to our new save game form.
	void onCancelPressed();
	void onNewSaveGamePressed();
};