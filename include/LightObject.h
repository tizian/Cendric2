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
	LightBean m_bean;
	float m_yScale;
	void init();
	
	// for testing
	sf::CircleShape m_ellipse;
	// for flickering
	sf::Time m_changingTime = sf::Time::Zero;
	const sf::Time INTERVAL_TIME = sf::seconds(1.f);
	bool m_isGrowing = true;
	const float MAX_SCALING = 0.6f;
};