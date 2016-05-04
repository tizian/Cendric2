#pragma once

#include "GameObjectComponents/TooltipComponent.h"

class LevelMainCharacter;

// Used for objects which Cendric can interact with. These include a tooltip, as well.
class InteractComponent : public virtual TooltipComponent {
public:
	InteractComponent(std::string tooltip, AnimatedGameObject* parent, LevelMainCharacter* mainChar);
	~InteractComponent() {}

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	void setOnInteract(const std::function<void()>& agent);
	void interact();
	void setInteractText(const std::string& textKey);
	void setInteractRange(float range);
	void setFocused(bool focused);

	bool getDistanceToMainChar() const;

private:
	// placeholder for agents
	void nop() const {};

private:
	bool m_isFocused;
	float m_interactRange;
	LevelMainCharacter* m_mainChar;
	std::string m_interactString;
	std::string m_tooltipString;
	std::function<void()> m_executeOnInteract;
};