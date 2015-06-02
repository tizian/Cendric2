#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include "GUI/Button.h"
#include "GUI/BitmapText.h"

// A form with two buttons on it, a Yes and a No button.
// The form is responsible for these two buttons (create, update, render, delete), they are NOT added to the screen objects, only the form is
class YesOrNoForm : public GameObject
{
public:
	YesOrNoForm(const sf::FloatRect& box, const sf::Color& pressedColor, const sf::Color& mousoverColor, const sf::Color& releasedColor);
	YesOrNoForm(const sf::FloatRect& box);
	~YesOrNoForm();

	void load() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	// position will be set automatically as the upper center of the form.
	void setMessage(Texts msg, const sf::Color& color);
	void setMessage(Texts msg);

	bool isYesClicked();
	bool isNoClicked();

	GameObjectType getConfiguredType() const override;

private:
	sf::RectangleShape m_shape;
	BitmapText m_message;
	Button* m_yesButton;
	Button* m_noButton;

	// determines the distance of the message text and the buttons from border
	const float DIST_FROM_BORDER = 10.f;
};