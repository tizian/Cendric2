#include "World/Camera/AutoscrollerCamera.h"
#include "InputController.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/Screen.h"

void AutoscrollerCamera::update(const sf::Time& frameTime) {
	Camera::update(frameTime);

	setCameraLeft(m_cameraLeft + frameTime.asSeconds() * m_autoscrollerSpeed, true);
	setCameraLeft(std::max(0.f, std::min(m_levelSize.x - m_cameraWindowWidth, m_cameraLeft)), true);
}

void AutoscrollerCamera::setAutoscrollerSpeed(int speed) {
	m_autoscrollerSpeed = static_cast<float>(speed);
}

void AutoscrollerCamera::setLevelSize(const sf::Vector2f& levelSize) {
	m_levelSize = levelSize;
}

void AutoscrollerCamera::setFocusCenter(const sf::Vector2f& center, bool setHard) {
	m_currentFocusCenter = center;
	// check vertically
	if (center.y < m_cameraTop) {
		setCameraTop(center.y, setHard);
	}
	else if (center.y > m_cameraTop + m_cameraWindowHeight) {
		setCameraTop(center.y - m_cameraWindowHeight, setHard);
	}
}
