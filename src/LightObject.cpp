#include "LightObject.h"

LightObject::LightObject(const LightBean& bean) : GameObject()
{
	m_bean = bean;

	init();
}

void LightObject::init()
{
	m_sprite.setSize(sf::Vector2f(2.f, 2.f));
	m_sprite.setOrigin(1.f, 1.f); // setting the origin to the center
	m_sprite.setScale(m_bean.radius.x, m_bean.radius.y);
	g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)->setSmooth(true);
	m_sprite.setTexture(g_resourceManager->getTexture(ResourceID::Texture_Particle_blob));
	m_animationTimer = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	setBoundingBox(sf::FloatRect(0.f, 0.f, 2.f * m_bean.radius.x, 2.f * m_bean.radius.y));

	setPosition(m_bean.center);
}

GameObjectType LightObject::getConfiguredType() const
{
	return GameObjectType::_Light;
}

void LightObject::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_sprite);
}

void LightObject::update(const sf::Time& frameTime)
{
	m_animationTimer += frameTime.asSeconds();

	float scaleX = m_bean.radius.x + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	float scaleY = m_bean.radius.y + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	
	m_sprite.setScale(scaleX, scaleY);
}

void LightObject::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos - sf::Vector2f(m_bean.radius.x, m_bean.radius.y));
	m_sprite.setPosition(pos);
}
