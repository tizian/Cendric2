#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "ResourceManager.h"
#include "Structs/LightData.h"

// A light ellipse in a level/map
class LightObject : public virtual GameObject {
public:
	LightObject(const LightData& data);
	virtual ~LightObject() {};

	virtual void render(sf::RenderTarget& renderTarget) override;
	virtual void update(const sf::Time& frameTime) override;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual GameObjectType getConfiguredType() const override;

	void setVisible(bool value);

protected:
	void init();
	bool m_isVisible = true;

	LightData m_lightData;

	sf::RectangleShape m_sprite;

	// Flickering animation
	float m_animationTimer;

	const float AMPLITUDE = 1.5f;
	const float FREQUENCY = 8.f;
};