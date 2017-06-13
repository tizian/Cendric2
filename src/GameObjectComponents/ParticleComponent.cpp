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
	if (!m_isVisible) return;
	m_ps->update(frameTime);
}

void ParticleComponent::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	if (m_data.particleTexture) {
		m_data.particleTexture->setView(renderTarget.getView());
		m_ps->render(*m_data.particleTexture);
	}
	else {
		m_ps->render(renderTarget);
	}
}

void ParticleComponent::emitParticles(int particles) {
	m_ps->emitParticles(particles);
}

void ParticleComponent::setVisible(bool visible) {
	m_isVisible = visible;
}

void ParticleComponent::setStatic(bool isStatic) {
	m_isStatic = isStatic;
}

void ParticleComponent::setEmitRate(float emitRate) {
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

void ParticleComponent::setGoalOffset(const sf::Vector2f& offset) {
	m_goalOffset = offset;
}

void ParticleComponent::setTexturePath(const std::string& texturePath) {
	auto tex = g_resourceManager->getTexture(m_data.texturePath);
	tex->setSmooth(true);
	m_ps->setTexture(tex);
	m_data.texturePath = texturePath;
}

void ParticleComponent::setPosition(const sf::Vector2f& pos) {
	if (m_isStatic) return;
	m_data.spawner->center = (m_parent->getPosition() + 
		sf::Vector2f(
			m_isOffsetFlippedX ? -m_offset.x + m_parent->getBoundingBox()->width : m_offset.x,
			m_isOffsetFlippedY ? -m_offset.y + m_parent->getBoundingBox()->height : m_offset.y));

	if (auto aimVel = dynamic_cast<particles::AimedVelocityGenerator*>(m_data.velGen)) {
		aimVel->goal = (m_parent->getPosition() +
			sf::Vector2f(
				m_isOffsetFlippedX ? -m_goalOffset.x + m_parent->getBoundingBox()->width : m_goalOffset.x,
				m_isOffsetFlippedY ? -m_goalOffset.y + m_parent->getBoundingBox()->height : m_goalOffset.y));
	}
}

void ParticleComponent::loadParticleSystem() {
	auto tex = g_resourceManager->getTexture(m_data.texturePath);
	tex->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(m_data.particleCount, tex);
	m_ps->additiveBlendMode = m_data.isAdditiveBlendMode;
	m_ps->emitRate = m_data.emitRate;
	m_ps->addSpawner(m_data.spawner);
	m_ps->addGenerator(m_data.sizeGen);
	m_ps->addGenerator(m_data.colorGen);
	m_ps->addGenerator(m_data.velGen);
	m_ps->addGenerator(m_data.timeGen);

	m_ps->addUpdater(m_data.colorUpdater ? m_data.colorUpdater : new particles::ColorUpdater());
	m_ps->addUpdater(m_data.eulerUpdater ? m_data.eulerUpdater : new particles::EulerUpdater());

	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

particles::ColorUpdater* ParticleComponent::getColorUpdater() const {
	return m_data.colorUpdater;
}

particles::ColorGenerator* ParticleComponent::getColorGenerator() const {
	return m_data.colorGen;
}

particles::TextureParticleSystem* ParticleComponent::getParticleSystem() const {
	return m_ps;
}