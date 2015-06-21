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
	Window(const sf::FloatRect& box, WindowOrnamentStyle style = WindowOrnamentStyle::NONE);

	void render(sf::RenderTarget& renderTarget) override;

	sf::Vector2f getSize() const;

	GameObjectType getConfiguredType() const override;

private:
	sf::Vector2f m_positionDefault;
	sf::Vector2f m_size;

	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;
};