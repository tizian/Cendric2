#pragma once

#include "global.h"

class Camera {
public:
	virtual ~Camera() {}

	virtual void update(const sf::Time& frameTime) = 0;

	void setCameraWindowHeight(float height);
	void setCameraWindowWidth(float width);
	virtual void setFocusCenter(const sf::Vector2f& center);
	const sf::Vector2f& getCameraCenter() const;

protected:
	sf::Vector2f m_cameraCenter;
	float m_cameraLeft = 0;
	float m_cameraTop = 0;
	float m_cameraWindowWidth = 0.f;
	float m_cameraWindowHeight = 0.f;

	// the speed the camera window is allowed to move when toggled by a user
	const float CAMERA_SPEED_PER_S = 200.f;
};