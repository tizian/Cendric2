#pragma once

#include "global.h"
#include "World/Camera/Camera.h"

class Screen;

class AutoscrollerCamera : public Camera {
public:
	void update(const sf::Time& frameTime) override;
	
	void setFocusCenter(const sf::Vector2f& center) override;
	void setScreen(Screen* character);
	void setAutoscrollerSpeed(int speed);
	void setCameraCenterX(float x);

private:
	float m_autoscrollerSpeed = 0;
	Screen* m_screen = nullptr;
};