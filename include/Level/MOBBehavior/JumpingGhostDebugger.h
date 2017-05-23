#pragma once

#include "global.h"
#include "World/GameObject.h"

// a game object that gets created by a jumping ghost and added to the screen to be rendered for debug purposes.
class JumpingGhostDebugger final : public GameObject {
public:
	JumpingGhostDebugger();
	~JumpingGhostDebugger();

	void addDebugBoundingBox(const sf::FloatRect& boundingBox);
	// marks a trajectory as successful, using green shapes instead of red ones.
	void setGoodTrajectory();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	GameObjectType getConfiguredType() const override;

protected:
	sf::Time m_renderTime;
	std::vector<sf::RectangleShape> m_debugBoxes;
};