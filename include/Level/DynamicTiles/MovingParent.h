#pragma once

#include "global.h"
#include "World/GameObject.h"

class MovingParent : public virtual GameObject {
public:
	MovingParent();
	virtual ~MovingParent();

	const sf::Vector2f& getRelativeVelocity() const;

protected:
	sf::Vector2f m_relativeVelocity;
};