#include "World/WeatherSystem.h"

#include "ResourceManager.h"
#include "Particles/ParticleGenerator.h"
#include "Particles/ParticleUpdater.h"
#include "GlobalResource.h"

void WeatherSystem::load(const WeatherData* data, bool isLevel) {
	if (data == nullptr) return;

	if (m_ps != nullptr) {
		delete m_ps;
	}

	if (data->weather == "rain") {
		m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_RAIN));

		m_ps->emitRate = 200.f;

		float minSize = isLevel ? 16.f : 10.f;
		float maxSize = isLevel ? 40.f : 30.f;

		auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
		sizeGen->minStartSize = minSize;
		sizeGen->maxStartSize = maxSize;
		sizeGen->minEndSize = minSize;
		sizeGen->maxEndSize = maxSize;

		auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 160.f;
		velGen->maxAngle = 160.f;
		velGen->minStartSpeed = 400.f;
		velGen->maxStartSpeed = 500.f;
	}
	else if (data->weather == "snow") {
		m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_SNOW));

		m_ps->emitRate = 100.f;

		auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
		sizeGen->minStartSize = 4.f;
		sizeGen->maxStartSize = 16.f;
		sizeGen->minEndSize = 4.f;
		sizeGen->maxEndSize = 16.f;

		auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 160.f;
		velGen->maxAngle = 200.f;
		velGen->minStartSpeed = 100.f;
		velGen->maxStartSpeed = 250.f;
	}
	else if (data->weather == "wind") {
		m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_WIND));

		m_ps->emitRate = 100.f;

		auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
		sizeGen->minStartSize = 0.f;
		sizeGen->maxStartSize = 0.f;
		sizeGen->minEndSize = 100.f;
		sizeGen->maxEndSize = 200.f;

		auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 90.f;
		velGen->maxAngle = 90.f;
		velGen->minStartSpeed = 100.f;
		velGen->maxStartSpeed = 250.f;

		m_ps->addGenerator<particles::DirectionDefinedRotationGenerator>();
	}
	else {
		return;
	}

	auto spawner = m_ps->addSpawner<particles::BoxSpawner>();
	spawner->size = sf::Vector2f(4.f * WINDOW_WIDTH, 2.f * WINDOW_HEIGHT);
	m_center = &spawner->center;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 5.f;
	timeGen->maxTime = 5.f;

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
	delete m_ps;
}

void WeatherSystem::update(const sf::Vector2f& center, const sf::Time& frameTime) {
	if (!m_ps) return;

	*m_center = center;
	m_ps->update(frameTime);
}

void WeatherSystem::render(sf::RenderTarget& renderTarget) {
	if (!m_ps) return;

	m_ps->render(renderTarget);
}