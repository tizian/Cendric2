#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include "GUI/Button.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

// A form with two buttons on it, a Yes and a No button.
// The form is responsible for these two buttons (create, update, render, delete), they are NOT added to the screen objects, only the form is
class YesOrNoForm final : public GameObject {
public:
	YesOrNoForm(const sf::FloatRect& box);
	~YesOrNoForm();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	// position will be set automatically as the upper center of the form.
	void setMessage(const std::string& msg, const sf::Color& color);
	void setMessage(const std::string& msg);

	void setOnYesClicked(const std::function<void()>& agent);
	void setOnNoClicked(const std::function<void()>& agent);

	GameObjectType getConfiguredType() const override;

private:
	Window* m_window;
	BitmapText m_message;
	Button* m_yesButton;
	Button* m_noButton;
};