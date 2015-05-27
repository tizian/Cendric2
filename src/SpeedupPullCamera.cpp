#include "SpeedupPullCamera.h"

void SpeedupPullCamera::setCameraWindowHeight(float height)
{
	m_cameraWindowHeight = height;
}

void SpeedupPullCamera::setCameraWindowWidth(float width) 
{
	m_cameraWindowWidth = width;
}

void SpeedupPullCamera::setFocusCenter(const sf::Vector2f& center)
{
	// check horizontal
	if (center.x < m_cameraLeft)
	{
		m_cameraLeft = center.x;
		m_cameraCenter.x = m_cameraLeft + (m_cameraWindowWidth / 2.f);
	}
	else if (center.x > m_cameraLeft + m_cameraWindowWidth)
	{
		m_cameraLeft = center.x - m_cameraWindowWidth;
		m_cameraCenter.x = m_cameraLeft + (m_cameraWindowWidth / 2.f);
	}

	// check vertical
	if (center.y < m_cameraTop)
	{
		m_cameraTop = center.y;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
	else if (center.y > m_cameraTop + m_cameraWindowHeight)
	{
		m_cameraTop = center.y - m_cameraWindowHeight;
		m_cameraCenter.y = m_cameraTop + (m_cameraWindowHeight / 2.f);
	}
}

const sf::Vector2f& SpeedupPullCamera::getCameraCenter() const
{
	return m_cameraCenter;
}