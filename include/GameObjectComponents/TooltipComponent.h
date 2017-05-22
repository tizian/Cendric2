#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "GUI/BitmapText.h"
#include "World/AnimatedGameObject.h"

// A game object component that holds a tooltip text
class TooltipComponent : public virtual GameObjectComponent {
public:
	TooltipComponent(const std::string& tooltip, AnimatedGameObject* parent, bool useInteractiveColor = true);
	TooltipComponent(const std::string& tooltip, GameObject* parent);
	virtual ~TooltipComponent() {}

	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setTooltipHeight(float height);
	void setTooltipText(const std::string& tooltip);
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void onParentMouseOver() override;

protected:
	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	AnimatedGameObject* m_animatedParent;
	bool m_useInteractiveColor;
	float m_tooltipHeight;

	static const sf::Time TOOLTIP_TIME;
};