#include "LightObject.h"

LightObject::LightObject(const LightBean& bean) : GameObject()
{
	m_bean = bean;

	init();
}

void LightObject::init()
{
	m_ellipse.setOrigin(m_bean.radiusX, m_bean.radiusX); // setting the origin to the center
	m_ellipse.setRadius(m_bean.radiusX);
	m_yScale = (float)m_bean.radiusY / (float)m_bean.radiusX;
	m_ellipse.setScale(1.f, m_yScale);
	m_ellipse.setFillColor(sf::Color(255, 255, 255, 100));

	setPosition(m_bean.center);
}

GameObjectType LightObject::getConfiguredType() const
{
	return GameObjectType::_Light;
}

void LightObject::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_ellipse);
}

void LightObject::update(const sf::Time& frameTime)
{
	GameObject::updateTime(m_changingTime, frameTime);
	if (m_changingTime == sf::Time::Zero)
	{
		m_changingTime = INTERVAL_TIME;
		m_isGrowing = !m_isGrowing;
	}

	float scaleX;
	float scaleY;
	if (m_isGrowing)
	{
		scaleX = ((1.f - MAX_SCALING) * 1) + MAX_SCALING * 1 * ((1 - (m_changingTime / INTERVAL_TIME)));
		scaleY = ((1.f - MAX_SCALING) * m_yScale) + MAX_SCALING * m_yScale * ((1 - (m_changingTime / INTERVAL_TIME)));
	}
	else
	{
		scaleX = ((1.f - MAX_SCALING) * 1) - MAX_SCALING * 1 * ((1 - (m_changingTime / INTERVAL_TIME)));
		scaleY = ((1.f - MAX_SCALING) * m_yScale) - MAX_SCALING * m_yScale * ((1 - (m_changingTime / INTERVAL_TIME)));
	}
	
	m_ellipse.setScale(scaleX, scaleY);
}

void LightObject::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos - sf::Vector2f(m_bean.radiusX, m_bean.radiusY));
	m_ellipse.setPosition(pos);
}
