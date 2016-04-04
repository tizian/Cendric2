#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/Button.h"
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

	virtual ~Window();

	virtual void update(const sf::Time& frameTime) override;
	virtual void render(sf::RenderTarget& renderTarget) override;

	void addCloseButton(const std::function<void()>& agent);

	void setSize(const sf::Vector2f& size) override;
	void setHeight(float height);
	void setWidth(float width);

	void setMainColor(const sf::Color& color);
	void setBackColor(const sf::Color& color);
	void setOrnamentColor(const sf::Color& color);

	virtual void setPosition(const sf::Vector2f& position) override;

	GameObjectType getConfiguredType() const override;

private:
	void init(const sf::FloatRect& box, WindowOrnamentStyle style);
	void updateCloseButton();

private:
	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;

	sf::Color m_mainColor;
	sf::Color m_backColor;
	sf::Color m_ornamentColor;

	Button *m_closeButton = nullptr;
};