#include "World/LightObject.h"
#include "GlobalResource.h"

LightObject::LightObject(const LightData& data) : GameObject() {
	m_lightData = data;

	init();
}

void LightObject::init() {
	m_sprite.setSize(sf::Vector2f(2.f, 2.f));
	m_sprite.setOrigin(1.f, 1.f); // setting the origin to the center
	m_sprite.setScale(2 * m_lightData.radius.x, 2 * m_lightData.radius.y);
	m_sprite.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(255 * m_lightData.brightness)));
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_LIGHT)->setSmooth(true);
	m_sprite.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_LIGHT));
	m_animationTimer = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	setBoundingBox(sf::FloatRect(0.f, 0.f, 2.f * m_lightData.radius.x, 2.f * m_lightData.radius.y));

	setPosition(m_lightData.center);

	float scaleX = m_lightData.radius.x + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	float scaleY = m_lightData.radius.y + AMPLITUDE * sin(FREQUENCY * m_animationTimer);

	m_sprite.setScale(scaleX, scaleY);
}

GameObjectType LightObject::getConfiguredType() const {
	return GameObjectType::_Light;
}

void LightObject::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(m_sprite);
}

void LightObject::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;
	m_animationTimer += frameTime.asSeconds();

	float scaleX = m_lightData.radius.x + AMPLITUDE * sin(FREQUENCY * m_animationTimer);
	float scaleY = m_lightData.radius.y + AMPLITUDE * sin(FREQUENCY * m_animationTimer);

	m_sprite.setScale(scaleX, scaleY);
}

void LightObject::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos - sf::Vector2f(m_lightData.radius.x, m_lightData.radius.y));
	m_sprite.setPosition(pos);
}

void LightObject::setVisible(bool value) {
	m_isVisible = value;
}