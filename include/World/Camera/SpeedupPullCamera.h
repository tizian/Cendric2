#pragma once

#include "global.h"
#include "World/Camera/Camera.h"

class SpeedupPullCamera : public Camera {
public:
	void update(const sf::Time& frameTime) override;

	void setFocusCenter(const sf::Vector2f& center) override;
};