#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

enum GUIElement : int {
	Character = 0,
	Inventory,
	Spellbook,
	Journal,
	Map
};

class GUITabButton : public GameObject {
public:
	GUITabButton(GUIElement element);

	void setActive(bool active);
	bool isActive() const;

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setOnClick(const std::function<void()>& agent);
	void setPosition(const sf::Vector2f& position) override;
	
	bool isClicked() const;

	GameObjectType getConfiguredType() const override;

public:
	static const float SIZE;
	static const float OFFSET;

protected:
	bool m_isActive = false;
	bool m_isMouseOver = false;
	bool m_isPressed = false;
	bool m_isClicked = false;

	sf::Sprite m_border;
	sf::Sprite m_icon;
	sf::CircleShape m_background;
	sf::CircleShape m_overlay;

	sf::Color m_backgroundColor = COLOR_TRANS_BLACK;
	sf::Color m_activeColor = COLOR_TRANS_GREY;
	sf::Color m_highlightColor = COLOR_PURPLE;

private:
	std::function<void()> m_executeOnClick;
	// placeholder for agents
	void nop() const;
};