#include "World/Camera/Camera.h"

void Camera::setFocusCenter(const sf::Vector2f& center) {
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