#pragma once

#include <map>

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

/* A screen overlay sprite */
class ScreenOverlay : public virtual GameObject {
public:
	// an active time of zero signalizes that this texture shall be active until it's set disposed
	ScreenOverlay(ResourceID texture, const sf::Time& activeTime);
	~ScreenOverlay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	GameObjectType getConfiguredType() const override;

private:
	sf::Sprite m_sprite;
	sf::Time m_activeTime;
};

