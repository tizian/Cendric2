#include "Level/Boss.h"

Boss::Boss(const Level* level, Screen* screen) : 
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	
	m_isBoss = true;
}

Boss::~Boss() {
	delete m_deathPs;
}

void Boss::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) {
		m_deathPs->update(frameTime);
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_deathPs->emitRate = 0;
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
	}
}

void Boss::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_isDead) m_deathPs->render(target);
}

void Boss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	updateDeathParticleSystemPosition();
}

void Boss::updateDeathParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2.f;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * (2.f / 3.f);
}

void Boss::loadDeathParticleSystem() {
	m_deathPs = new particles::TextureParticleSystem(300, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR));
	m_deathPs->additiveBlendMode = true;
	m_deathPs->emitRate = 100.f;

	// Generators
	auto spawner = m_deathPs->addSpawner<particles::DiskSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->radius = 20.f;
	m_particleSpawner = spawner;

	auto sizeGen = m_deathPs->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_deathPs->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_deathPs->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;

	auto timeGen = m_deathPs->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_deathPs->addUpdater<particles::TimeUpdater>();
	m_deathPs->addUpdater<particles::ColorUpdater>();
	m_deathPs->addUpdater<particles::EulerUpdater>();
	m_deathPs->addUpdater<particles::SizeUpdater>();
}

std::string Boss::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
}
