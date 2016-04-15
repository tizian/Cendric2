#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "GUI/SlotTooltipWindow.h"

class Slot : public GameObject {
	friend class SlotClone;
public:
	Slot() {};
	virtual ~Slot() {};

	void initSlot();

	void setPosition(const sf::Vector2f& pos) override;

	virtual void select();
	virtual void deselect();

	void activate();
	void deactivate();

	void highlight();
	void unhighlight();

	virtual void update(const sf::Time& frameTime) override;

	void render(sf::RenderTarget& renderTarget) override;
	virtual void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	virtual void onLeftJustPressed() override;
	virtual void onRightClick() override;
	virtual void onMouseOver() override;

	inline bool isClicked() const { return m_isClicked; }
	inline bool isRightClicked() const { return m_isRightClicked; }

	GameObjectType getConfiguredType() const override;

	virtual float getConfiguredSize() const = 0;
	virtual float getConfiguredIconOffset() const = 0;

	static const float ICON_SIZE;
	static const float TOOLTIP_TOP;

protected:
	bool m_isClicked = false;
	bool m_isRightClicked = false;

	bool m_isEmpty = false;
	bool m_isSelected = false;

	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_iconRect;
	sf::RectangleShape m_overlayRect;
	sf::RectangleShape m_borderRect;

	sf::IntRect m_iconTextureRect;
	const sf::Texture* m_iconTexture = nullptr;
	const sf::Texture* m_borderTexture = nullptr;			// TODO: Maybe use one single spritesheet for all "slot border" textures
	const sf::Texture* m_borderTextureSelected = nullptr;
	const sf::Texture* m_highlightTexture = nullptr;

	bool m_showTooltip = false;
	SlotTooltipWindow m_tooltipWindow;
};