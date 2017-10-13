#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "ResourceManager.h"
#include "GUI/Button.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

// A form with two buttons on it, an ok and a cancel button.
// additionally, it takes text input from the user for the savegame name.
class NewSaveGameForm final : public GameObject {
public:
	NewSaveGameForm(const sf::FloatRect& box);
	~NewSaveGameForm();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	// this name is not cleared of invalid chars for a filename!
	const std::string& getSavegameName() const;

	void setOnOkClicked(const std::function<void()>& agent);
	void setOnCancelClicked(const std::function<void()>& agent);

	GameObjectType getConfiguredType() const override;

private:
	Window* m_window;
	BitmapText m_message;
	Button* m_okButton;
	Button* m_cancelButton;

	std::function<void()> m_executeOnOk = NOP;

	std::string m_savegameName;
	BitmapText m_savegameNameText;
};