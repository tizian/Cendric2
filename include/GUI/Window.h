#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

enum class WindowOrnamentStyle {
	NONE,
	SIMPLE,
	FANCY
};

class Window : public GameObject {
public:
	Window(const sf::FloatRect& box, WindowOrnamentStyle style);
	Window(const sf::FloatRect& box, WindowOrnamentStyle style, const sf::Color& mainColor, const sf::Color& backColor, const sf::Color& ornamentColor);

	virtual void render(sf::RenderTarget& renderTarget) override;

	void setSize(const sf::Vector2f& size) override;
	void setHeight(float height);
	void setWidth(float width);

	virtual void setPosition(const sf::Vector2f& position) override;

	GameObjectType getConfiguredType() const override;

private:
	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;
};