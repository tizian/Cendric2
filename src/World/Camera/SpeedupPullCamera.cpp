#include "World/Camera/SpeedupPullCamera.h"

void SpeedupPullCamera::setFocusCenter(const sf::Vector2f& center, bool setHard) {
	m_currentFocusCenter = center;
	// check horizontally
	if (center.x < m_cameraLeft) {
		setCameraLeft(center.x, setHard);
	}
	else if (center.x > m_cameraLeft + m_cameraWindowWidth) {
		setCameraLeft(center.x - m_cameraWindowWidth, setHard);
	}

	// check vertically
	if (center.y < m_cameraTop) {
		setCameraTop(center.y, setHard);
	}
	else if (center.y > m_cameraTop + m_cameraWindowHeight) {
		setCameraTop(center.y - m_cameraWindowHeight, setHard);
	}
}