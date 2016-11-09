#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include "GUI/Button.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

// A form with an ok button
// it takes text input from the user for the user's id for the party
class PartyForm : public GameObject {
public:
	PartyForm(const sf::FloatRect& box);
	~PartyForm();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	const std::string& getEnteredString() const;

	void setOnOkClicked(const std::function<void()>& agent);

	GameObjectType getConfiguredType() const override;

private:
	Window* m_window;
	BitmapText m_message;
	Button* m_okButton;

	std::function<void()> m_executeOnOk;
	void nop() const {};

	std::string m_enteredString;
	BitmapText m_enteredStringText;
};