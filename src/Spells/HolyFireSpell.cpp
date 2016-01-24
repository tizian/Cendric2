#include "Spells/HolyFireSpell.h"

HolyFireSpell::HolyFireSpell() : Spell() {
}

void HolyFireSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_lightObject = new LightObject(LightData(sf::Vector2f(), bean.range));
	loadParticleSystem();
}

sf::Vector2f HolyFireSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-m_data.range, -m_data.range);
}

void HolyFireSpell::setDisposed() {
	Spell::setDisposed();
	m_lightObject->setDisposed();
}

void HolyFireSpell::execOnHit(LevelMovableGameObject* target) {
	m_hasDamaged = true;
}

void HolyFireSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void HolyFireSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	if (m_lightObject != nullptr) m_lightObject->setPosition(pos + sf::Vector2f(m_data.range, m_data.range));
	updateParticleSystemPosition();
}

void HolyFireSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void HolyFireSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);
	if (m_emitTime > sf::Time::Zero) {
		GameObject::updateTime(m_emitTime, frameTime);
		if (m_emitTime == sf::Time::Zero) {
			m_ps->emitRate = 0.f;
		}
	}
	if (m_hasDamaged) {
		m_data.damageType = DamageType::VOID;
	}
}

void HolyFireSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(static_cast<int>(m_data.range), g_resourceManager->getTexture(ResourceID::Texture_Particle_flame)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = m_data.range;

	// Generators
	auto posGen = m_ps->addGenerator<particles::DiskPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->radius = m_data.range;
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 30.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(162, 7, 0, 50);
	colGen->maxStartCol = sf::Color(234, 148, 0, 100);
	colGen->minEndCol =  sf::Color(255, 244, 127, 30);
	colGen->maxEndCol = sf::Color(255, 255, 255, 50);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -20.f;
	velGen->maxAngle = 20.f;
	velGen->minStartVel = 10.f;
	velGen->maxStartVel = 30.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void HolyFireSpell::updateParticleSystemPosition() {
	if (m_posGenerator == nullptr) return;
	m_posGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_posGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

