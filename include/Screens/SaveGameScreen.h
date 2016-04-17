#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "GUI/SaveGameWindow.h"
#include "GUI/YesOrNoForm.h"
#include "GUI/NewSaveGameForm.h"

class SaveGameScreen : public Screen {
public:
	SaveGameScreen(CharacterCore* core);
	~SaveGameScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	void setAllButtonsEnabled(bool value) override;

	Button* m_saveButton = nullptr;
	Button* m_deleteSaveGameButton = nullptr;
	BitmapText* m_title = nullptr;

	SaveGameWindow* m_saveGameWindow = nullptr;
	NewSaveGameForm* m_newSaveGameForm = nullptr;
	YesOrNoForm* m_yesOrNoForm = nullptr;

	// pretty little agents to give to our yes or no form.
	void onNo();
	void onYesOverwriteSaveGame();
	void onYesDeleteSaveGame();

	// pretty little agents to give to our new save game form.
	void onCancel();
	void onYesNewSaveGame();

	// pretty little button agents :3
	void onDeleteSaveGame();
	void onNewSaveGame();
	void onBack();
	void onSaveGame();
};