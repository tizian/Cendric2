#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include "GUI/Button.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

// A form with two buttons on it, an ok and a cancel button.
// additionally, it takes text input from the user for the savegame name.
class NewSaveGameForm : public GameObject
{
public:
	NewSaveGameForm(const sf::FloatRect& box);
	~NewSaveGameForm();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	bool isCancelClicked();
	// if this is true, it is guaranteed that the savegame name is set correctly.
	bool isOkClicked();
	// this name is not cleared of invalid chars for a filename!
	const std::string& getSavegameName() const;

	GameObjectType getConfiguredType() const override;

private:
	Window* m_window;
	BitmapText m_message;
	Button* m_okButton;
	Button* m_cancelButton;

	std::string m_savegameName;
	BitmapText m_savegameNameText;
};