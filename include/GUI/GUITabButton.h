#pragma once

#include "global.h"
#include "GUI/ButtonInterface.h"
#include "GUI/BitmapText.h"

class GUITabButton final : public ButtonInterface {
public:
	GUITabButton();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setOnClick(const std::function<void()>& agent);
	void setPosition(const sf::Vector2f& position) override;
	void setTexture(const sf::Texture* tex, int x);
	void setText(const std::string& text);

	void click() override;

protected:
	void updateColor() override;

public:
	static const int ICON_SIZE;
	static const int SIZE;
	static const int OFFSET;

protected:
	sf::Sprite m_border;
	sf::Sprite m_icon;
	sf::CircleShape m_background;

	BitmapText m_inputKey;

	sf::Color m_backgroundColor = COLOR_TRANS_BLACK;
	sf::Color m_selectedColor = COLOR_TRANS_GREY;
	sf::Color m_highlightColor = COLOR_PURPLE;

private:
	std::function<void()> m_executeOnClick = NOP;
};