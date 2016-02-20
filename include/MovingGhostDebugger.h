#pragma once

#include "global.h"
#include "GameObject.h"

// a game object that gets created by a movable ghost and added to the screen to be rendered for debug purposes.
class MovingGhostDebugger : public virtual GameObject {
public:
	MovingGhostDebugger();
	~MovingGhostDebugger();

	void addDebugBoundingBox(const sf::FloatRect& boundingBox);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	GameObjectType getConfiguredType() const override;

protected:
	sf::Time m_renderTime;
	std::vector<sf::RectangleShape> m_debugBoxes;
};