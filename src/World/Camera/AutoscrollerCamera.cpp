#include "World/Camera/AutoscrollerCamera.h"
#include "InputController.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/Screen.h"

void AutoscrollerCamera::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Up)) {
		m_cameraTop -= CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	if (g_inputController->isKeyActive(Key::Down)) {
		m_cameraTop += CAMERA_SPEED_PER_S * frameTime.asSeconds();
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}

	m_cameraCenter.x += frameTime.asSeconds() * m_autoscrollerSpeed;
	m_cameraLeft = m_cameraCenter.x - WINDOW_WIDTH * 0.5f;
}

void AutoscrollerCamera::setAutoscrollerSpeed(int speed) {
	m_autoscrollerSpeed = static_cast<float>(speed);
}

void AutoscrollerCamera::setCameraCenterX(float x) {
	m_cameraCenter.x = x;
}

void AutoscrollerCamera::setLevelSize(const sf::Vector2f& levelSize) {
	m_levelSize = levelSize;
}

void AutoscrollerCamera::setFocusCenter(const sf::Vector2f& center) {
	// check vertically
	if (center.y < m_cameraTop) {
		m_cameraTop = center.y;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	else if (center.y > m_cameraTop + m_cameraWindowHeight) {
		m_cameraTop = center.y - m_cameraWindowHeight;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}

	m_cameraLeft = std::min(m_levelSize.x - WINDOW_WIDTH, m_cameraLeft);
}
