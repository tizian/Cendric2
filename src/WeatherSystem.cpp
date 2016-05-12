#include "WeatherSystem.h"

#include "ResourceManager.h"
#include "Particles/ParticleGenerator.h"
#include "Particles/ParticleUpdater.h"

void WeatherSystem::load(const WeatherData* data) {
	if (data == nullptr) return;

	if (m_ps != nullptr) {
		delete m_ps;
	}

	if (data->weather.compare("rain") == 0) {
		m_ps = new particles::TextureParticleSystem(2500, g_resourceManager->getTexture(ResourceID::Texture_Particle_rain));

		m_ps->emitRate = 200.f;

		auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
		sizeGen->minStartSize = 5.f;
		sizeGen->maxStartSize = 15.f;
		sizeGen->minEndSize = 5.f;
		sizeGen->maxEndSize = 15.f;

		auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 160.f;
		velGen->maxAngle = 160.f;
		velGen->minStartVel = 400.f;
		velGen->maxStartVel = 500.f;
	}
	else if (data->weather.compare("snow") == 0) {
		m_ps = new particles::TextureParticleSystem(2500, g_resourceManager->getTexture(ResourceID::Texture_Particle_snow));

		m_ps->emitRate = 100.f;

		auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
		sizeGen->minStartSize = 2.f;
		sizeGen->maxStartSize = 8.f;
		sizeGen->minEndSize = 2.f;
		sizeGen->maxEndSize = 8.f;

		auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 160.f;
		velGen->maxAngle = 200.f;
		velGen->minStartVel = 100.f;
		velGen->maxStartVel = 250.f;
	}
	else {
		return;
	}

	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->size = sf::Vector2f(2.f * WINDOW_WIDTH, WINDOW_HEIGHT);
	m_center = &posGen->center;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 10.f;
	timeGen->maxTime = 10.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	m_ps->addUpdater<particles::SizeUpdater>();
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

WeatherSystem::~WeatherSystem() {
	if (m_ps) {
		delete m_ps;
	}
}

void WeatherSystem::update(const sf::Vector2f& center, const sf::Time& frameTime) {
	if (m_ps == nullptr) return;

	*m_center = center;
	m_ps->update(frameTime);
}

void WeatherSystem::render(sf::RenderTarget& renderTarget) {
	if (m_ps == nullptr) return;

	m_ps->render(renderTarget);
}