#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "GUI/BitmapText.h"
#include "GUI/TooltipWindow.h"

// A game object component that holds a tooltip in a window
class TooltipWindowComponent final : public virtual GameObjectComponent {
public:
	TooltipWindowComponent(const std::string& tooltip, GameObject* parent);

	void setPosition(const sf::Vector2f& pos) override;
	void setWindowOffset(const sf::Vector2f& offset);
	void setMaxWidth(float width);
	void setTextAlignment(TextAlignment alignment);
	void setTooltipText(const std::string& tooltip);
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void onParentMouseOver() override;

	float getWidth() const;

protected:
	bool m_showTooltip = false;
	TooltipWindow m_tooltipWindow;

	sf::Vector2f m_offset;
};