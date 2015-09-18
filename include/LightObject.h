#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Structs/LightBean.h"

// A light ellipse in a level/map
class LightObject : virtual public GameObject
{
public:
	LightObject(const LightBean& bean);
	virtual ~LightObject() {};

	virtual void render(sf::RenderTarget& renderTarget) override;
	virtual void update(const sf::Time& frameTime) override;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual GameObjectType getConfiguredType() const override;

protected:
	void init();

	LightBean m_bean;

	sf::RectangleShape m_sprite;
	
	// Flickering animation
	float m_animationTimer;

	const float AMPLITUDE = 1.f;
	const float FREQUENCY = 10.f;
};