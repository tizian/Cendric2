#pragma once

#include "global.h"
#include "World/GameObject.h"

#include "GUI/BitmapText.h"
#include "GUI/ButtonInterface.h"

class TooltipWindowComponent;

class Slot : virtual public GameObject, virtual public ButtonInterface {
	friend class SlotClone;
public:
	Slot();
	virtual ~Slot() = default;

	void initSlot();

	void setPosition(const sf::Vector2f& pos) override;

	void click() override;
	virtual void select();
	virtual void deselect();

	void activate();
	void deactivate();

	void highlight();
	void unhighlight();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void onLeftJustPressed() override;
	void onRightClick() override;
	void onMouseOver() override;

	bool isRightClicked() const { return m_isRightClicked; }
	bool isDoubleClicked() const { return m_isDoubleClicked; }

	bool isEmpty() const { return m_isEmpty; }

	GameObjectType getConfiguredType() const override;

	virtual float getConfiguredSize() const = 0;
	virtual float getConfiguredIconOffset() const = 0;

	static const float ICON_SIZE;
	static const float TOOLTIP_TOP;

protected:
	void updateColor() override;
	void notifyFirstSelection() override;

	bool m_isRightClicked = false;
	bool m_isEmpty = false;
	bool m_isDoubleClicked = false;

	sf::Time m_doubleClickTime = sf::Time::Zero;

	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_iconRect;
	sf::RectangleShape m_overlayRect;
	sf::RectangleShape m_borderRect;

	sf::IntRect m_iconTextureRect;
	const sf::Texture* m_iconTexture = nullptr;
	const sf::Texture* m_borderTexture = nullptr;		
	const sf::Texture* m_borderTextureSelected = nullptr;
	const sf::Texture* m_highlightTexture = nullptr;

	static const sf::Time DOUBLE_CLICK_TIME;

	TooltipWindowComponent* m_tooltipComponent;
	virtual void adjustTooltipOffset();
};