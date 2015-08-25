#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

enum class WindowOrnamentStyle {
	NONE,
	SMALL,
	MEDIUM,
	LARGE
};

class Window : public GameObject
{
public:
	Window(const sf::FloatRect& box, WindowOrnamentStyle style);
	Window(const sf::FloatRect& box, WindowOrnamentStyle style, const sf::Color& mainColor, const sf::Color& backColor, const sf::Color& ornamentColor);

	virtual void render(sf::RenderTarget& renderTarget) override;

	void setHeight(float height);
	void setWidth(float width);
	const sf::Vector2f& getSize() const;
	virtual void setPosition(const sf::Vector2f& position) override;

	GameObjectType getConfiguredType() const override;

private:
	sf::Vector2f m_size;

	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;
};