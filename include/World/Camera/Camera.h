#pragma once

#include "global.h"

class Camera {
public:
	virtual ~Camera() {}

	virtual void update(const sf::Time& frameTime) = 0;

	void setCameraWindowHeight(float height);
	void setCameraWindowWidth(float width);
	virtual void setFocusCenter(const sf::Vector2f& center, bool setHard = false);
	const sf::Vector2f& getCameraCenter() const;

	float getCameraLeft() const { return m_cameraLeft; }

protected:
	void setCameraLeft(float cameraLeft, bool setHard);
	void setCameraTop(float cameraTop, bool setHard);

	sf::Vector2f m_cameraCenter;
	float m_cameraLeft = 0;
	float m_cameraTop = 0;
	float m_cameraWindowWidth = 0.f;
	float m_cameraWindowHeight = 0.f;
	sf::Time m_currentFrameTime;

	// the speed the camera window is allowed to move when toggled by a user
	static const float CAMERA_SPEED_PER_S;
	static const float CAMERA_LERP_SPEED;
};