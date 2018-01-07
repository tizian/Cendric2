#pragma once

#include "global.h"

class LevelMainCharacter;

class Camera {
public:
	Camera(float width, float height);
	virtual ~Camera() {}

	virtual void update(const sf::Time& frameTime);

	virtual void setFocusCenter(const sf::Vector2f& center, bool setHard = false);
	const sf::Vector2f& getCameraCenter() const;

	float getCameraLeft() const { return m_cameraCenter.x - WINDOW_WIDTH * 0.5f; }
	float getCameraTop() const { return m_cameraCenter.y - WINDOW_HEIGHT * 0.5f; }

	void setCameraCenterX(float centerX, bool setHard);
	void setCameraCenterY(float centerY, bool setHard);
	void setCameraLeft(float cameraLeft, bool setHard);
	void setCameraTop(float cameraTop, bool setHard);

	void setLevelMainCharacter(LevelMainCharacter* mainChar);

protected:
	LevelMainCharacter* m_mainChar = nullptr;
	sf::Vector2f m_currentFocusCenter;
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