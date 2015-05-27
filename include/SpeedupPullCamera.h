#pragma once

#include "global.h"
#include "BackgroundLayer.h"

class SpeedupPullCamera
{
public:

	void setCameraWindowHeight(float height);
	void setCameraWindowWidth(float width);
	void setFocusCenter(const sf::Vector2f& center);

	const sf::Vector2f& getCameraCenter() const;

private:
	sf::Vector2f m_cameraCenter;
	float m_cameraWindowWidth = 0.f;
	float m_cameraWindowHeight = 0.f;
	float m_cameraLeft = 0;
	float m_cameraTop = 0;
};