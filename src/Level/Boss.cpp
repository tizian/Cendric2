#include "Level/Boss.h"
#include "GameObjectComponents/ParticleComponent.h"

Boss::Boss(const Level* level, Screen* screen) : 
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	
	m_isAlwaysUpdate = true;
	m_isBoss = true;
}

void Boss::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) {
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_deathPc->setEmitRate(0.f);
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
	}
}

void Boss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	m_deathPc->setVisible(true);
}

void Boss::loadDeathParticles() {
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	
	// Generators
	auto spawner = new particles::DiskSpawner();
	spawner->radius = 20.f;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;
	data.timeGen = timeGen;

	m_deathPc = new ParticleComponent(data, this);
	m_deathPc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.8f));
	m_deathPc->setVisible(false);
	addComponent(m_deathPc);
}

std::string Boss::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
}
