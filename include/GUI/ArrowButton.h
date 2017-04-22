#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

class ArrowButton final : virtual public GameObject {
public:
	ArrowButton(bool pointRight);

	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setEnabled(bool value);

	void setMouseoverColor(const sf::Color& color);
	void setMainColor(const sf::Color& color);
	void setDisabledColor(const sf::Color& color);

	bool isClicked() const;

	GameObjectType getConfiguredType() const override;

private:
	void recalculateColor();

	bool m_isMouseOver = false;
	bool m_wasMouseOver = false;
	bool m_isClicked = false;
	bool m_isEnabled = false;

	sf::Sprite m_arrow;

	sf::Color m_mouseoverColor;
	sf::Color m_mainColor;
	sf::Color m_disabledColor;
};
