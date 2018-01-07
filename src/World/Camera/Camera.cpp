#include "World/Camera/Camera.h"

const float Camera::CAMERA_SPEED_PER_S = 200.f;
const float Camera::CAMERA_LERP_SPEED = 5.f;

void Camera::setFocusCenter(const sf::Vector2f& center, bool setHard) {
	m_cameraCenter = center;
}

const sf::Vector2f& Camera::getCameraCenter() const {
	return m_cameraCenter;
}

void Camera::setCameraWindowHeight(float height) {
	m_cameraWindowHeight = height;
}

void Camera::setCameraWindowWidth(float width) {
	m_cameraWindowWidth = width;
}

void Camera::setCameraTop(float cameraTop, bool setHard) {
	m_cameraTop = setHard ? cameraTop :
		lerp(m_currentFrameTime.asSeconds() * CAMERA_LERP_SPEED, m_cameraTop, cameraTop);

	m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight * 0.5f);
}

void Camera::setCameraLeft(float cameraLeft, bool setHard) {
	m_cameraLeft = setHard ? cameraLeft :
		lerp(m_currentFrameTime.asSeconds() * CAMERA_LERP_SPEED, m_cameraLeft, cameraLeft);

	m_cameraCenter.x = m_cameraLeft + (m_cameraWindowWidth * 0.5f);
}