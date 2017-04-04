#include "GameObjectComponents/ParticleComponent.h"
#include "Screens/Screen.h"

ParticleComponent::ParticleComponent(const ParticleComponentData& data, GameObject* parent) : 
	GameObjectComponent(parent), m_data(data) {
	loadParticleSystem();
}

ParticleComponent::~ParticleComponent() {
	delete m_ps;
}

void ParticleComponent::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
}

void ParticleComponent::render(sf::RenderTarget& renderTarget) {
	m_ps->render(renderTarget);
}

void ParticleComponent::setVisible(bool visible) {
	m_isVisible = visible;
}

void ParticleComponent::setEmitRate(bool emitRate) {
	m_data.emitRate = emitRate;
	m_ps->emitRate = emitRate;
}

void ParticleComponent::flipOffsetX(bool flipped) {
	m_isOffsetFlippedX = flipped;
}

void ParticleComponent::flipOffsetY(bool flipped) {
	m_isOffsetFlippedY = flipped;
}

void ParticleComponent::setOffset(const sf::Vector2f& offset) {
	m_offset = offset;
}

void ParticleComponent::setPosition(const sf::Vector2f& pos) {
	m_data.spawner->center = (m_parent->getPosition() + 
		sf::Vector2f(
			m_isOffsetFlippedX ? -m_offset.x + m_parent->getBoundingBox()->width : m_offset.x,
			m_isOffsetFlippedY ? -m_offset.y + m_parent->getBoundingBox()->height : m_offset.y));
}

void ParticleComponent::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(m_data.particleCount, g_resourceManager->getTexture(m_data.texturePath));
	m_ps->additiveBlendMode = m_data.isAdditiveBlendMode;
	m_ps->emitRate = m_data.emitRate;
	m_ps->addSpawner(m_data.spawner);
	m_ps->addGenerator(m_data.sizeGen);
	m_ps->addGenerator(m_data.colorGen);
	m_ps->addGenerator(m_data.velGen);
	m_ps->addGenerator(m_data.timeGen);

	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}