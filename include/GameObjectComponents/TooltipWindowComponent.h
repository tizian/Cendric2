#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "GUI/BitmapText.h"
#include "GUI/TooltipWindow.h"

// A game object component that holds a tooltip in a window
class TooltipWindowComponent final : public GameObjectComponent {
public:
	TooltipWindowComponent(const std::string& tooltip, GameObject* parent);

	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setWindowOffset(const sf::Vector2f& offset);
	void setMaxWidth(int width);
	void setTooltipTime(const sf::Time& time);
	void setCurrentTooltipTime(const sf::Time& time);
	void setTextAlignment(TextAlignment alignment);
	void setTextOffset(const sf::Vector2f& offset);
	void setTooltipText(const std::string& tooltip);
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void onParentMouseOver() override;

	bool isShowingTooltip() const;
	float getWidth() const;
	float getHeight() const;

protected:
	bool m_showTooltip = false;
	TooltipWindow m_tooltipWindow;
	sf::Time m_tooltipTime;
	sf::Time m_currentTooltipTime;

	sf::Vector2f m_offset;
};