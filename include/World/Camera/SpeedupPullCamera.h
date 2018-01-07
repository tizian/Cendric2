#pragma once

#include "global.h"
#include "World/Camera/Camera.h"

class SpeedupPullCamera : public Camera {
public:
	SpeedupPullCamera(float width, float height) : Camera(width, height) {};
	void setFocusCenter(const sf::Vector2f& center, bool setHard) override;
};