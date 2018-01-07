#include "World/Camera/Camera.h"
#include "InputController.h"
#include "Level/LevelMainCharacter.h"

const float Camera::CAMERA_SPEED_PER_S = 200.f;
const float Camera::CAMERA_LERP_SPEED = 5.f;

Camera::Camera(float width, float height) {
	m_cameraWindowWidth = width;
	m_cameraWindowHeight = height;
}

void Camera::update(const sf::Time& frameTime) {
	m_currentFrameTime = frameTime;
	if (g_inputController->isKeyActive(Key::Up)) {
		auto newCenterY = m_cameraCenter.y - CAMERA_SPEED_PER_S * frameTime.asSeconds();
		if (newCenterY > m_currentFocusCenter.y - m_cameraWindowHeight * 0.5f)
			setCameraCenterY(newCenterY, !m_mainChar->isClimbing());
	}
	if (g_inputController->isKeyActive(Key::Down)) {
		auto newCenterY = m_cameraCenter.y + CAMERA_SPEED_PER_S * frameTime.asSeconds();
		if (newCenterY < m_currentFocusCenter.y + m_cameraWindowHeight * 0.5f)
			setCameraCenterY(newCenterY, !m_mainChar->isClimbing());
	}
}

void Camera::setFocusCenter(const sf::Vector2f& center, bool setHard) {
	m_currentFocusCenter = center;
	m_cameraCenter = center;
}

const sf::Vector2f& Camera::getCameraCenter() const {
	return m_cameraCenter;
}

void Camera::setCameraCenterX(float centerX, bool setHard) {
	setCameraLeft(centerX - m_cameraWindowWidth * 0.5f, setHard);
}

void Camera::setCameraCenterY(float centerY, bool setHard) {
	setCameraTop(centerY - m_cameraWindowHeight * 0.5f, setHard);
}

void Camera::setLevelMainCharacter(LevelMainCharacter* mainChar) {
	m_mainChar = mainChar;
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