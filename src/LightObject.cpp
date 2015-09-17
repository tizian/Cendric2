#include "LightObject.h"

LightObject::LightObject(const LightBean& bean) : GameObject()
{
	m_bean = bean;

	init();
}

void LightObject::init()
{
	m_ellipse.setOrigin(1.f, 1.f); // setting the origin to the center
	m_ellipse.setRadius(1.f);
	m_ellipse.setScale(m_bean.radius.x, m_bean.radius.y);
	m_ellipse.setFillColor(sf::Color(255, 255, 255, 100));
	m_animationTimer = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

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
	m_animationTimer += frameTime.asSeconds();

	float scaleX = m_bean.radius.x + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	float scaleY = m_bean.radius.y + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	
	m_ellipse.setScale(scaleX, scaleY);
}

void LightObject::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos - sf::Vector2f(m_bean.radius.x, m_bean.radius.y));
	m_ellipse.setPosition(pos);
}
