#include "World/Camera/SpeedupPullCamera.h"
#include "InputController.h"

void SpeedupPullCamera::update(const sf::Time& frameTime) {
	m_currentFrameTime = frameTime;
	if (g_inputController->isKeyActive(Key::Up)) {
		m_cameraTop -= CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	if (g_inputController->isKeyActive(Key::Down)) {
		m_cameraTop += CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
}

void SpeedupPullCamera::setFocusCenter(const sf::Vector2f& center, bool setHard) {
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