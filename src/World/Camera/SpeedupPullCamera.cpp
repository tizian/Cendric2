#include "World/Camera/SpeedupPullCamera.h"
#include "InputController.h"

void SpeedupPullCamera::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Up)) {
		m_cameraTop -= CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	if (g_inputController->isKeyActive(Key::Down)) {
		m_cameraTop += CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
}

void SpeedupPullCamera::setFocusCenter(const sf::Vector2f& center) {
	// check horizontally
	if (center.x < m_cameraLeft) {
		m_cameraLeft = center.x;
		m_cameraCenter.x = m_cameraLeft + (m_cameraWindowWidth / 2.f);
	}
	else if (center.x > m_cameraLeft + m_cameraWindowWidth) {
		m_cameraLeft = center.x - m_cameraWindowWidth;
		m_cameraCenter.x = m_cameraLeft + (m_cameraWindowWidth / 2.f);
	}

	// check vertically
	if (center.y < m_cameraTop) {
		m_cameraTop = center.y;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	else if (center.y > m_cameraTop + m_cameraWindowHeight) {
		m_cameraTop = center.y - m_cameraWindowHeight;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
}