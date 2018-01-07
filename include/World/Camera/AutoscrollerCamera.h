#pragma once

#include "global.h"
#include "World/Camera/Camera.h"

class Screen;

class AutoscrollerCamera : public Camera {
public:
	void update(const sf::Time& frameTime) override;
	
	void setFocusCenter(const sf::Vector2f& center, bool setHard) override;
	void setAutoscrollerSpeed(int speed);
	void setLevelSize(const sf::Vector2f& levelSize);

private:
	float m_autoscrollerSpeed = 0;
	sf::Vector2f m_levelSize;
};