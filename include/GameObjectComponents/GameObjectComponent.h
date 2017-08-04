#pragma once

#include "global.h"
#include "World/GameObject.h"

// A component for a game object, will be updated and disposed with it.
// Examples are LightComponent which holds a LightObject or TooltipComponent.
class GameObjectComponent {
public:
	GameObjectComponent(GameObject* parent);
	virtual ~GameObjectComponent() {}

	virtual void update(const sf::Time& frameTime) {}
	virtual void setPosition(const sf::Vector2f& pos) {}
	virtual void render(sf::RenderTarget& renderTarget) {}
	virtual void renderAfterForeground(sf::RenderTarget& renderTarget) {}
	virtual void onParentMouseOver() {}
	virtual void onDisposed() {}
	void setActive(bool active) { m_isActive = active; }
	bool isActive() const { return m_isActive; }

protected:
	GameObject* m_parent = nullptr;
	bool m_isActive = true;
};