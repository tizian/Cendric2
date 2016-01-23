#include "Spells/LeapOfFaithSpell.h"
#include "Level/MOBBehavior/MovingBehavior.h"

LeapOfFaithSpell::LeapOfFaithSpell(float gravityScale) : Spell() {
	m_gravityScale = gravityScale;
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(100.f, 100.f), 0.6f));
}

void LeapOfFaithSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	m_mob = mob;
	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_leapoffaith));
	spellAnimation.addFrame(sf::IntRect(0, 0, 80, 120));

	addAnimation(GameObjectState::Idle, spellAnimation);

	m_isFacingRight = m_mob->isFacingRight();
	setCurrentAnimation(getAnimation(GameObjectState::Idle), !m_isFacingRight);
	playCurrentAnimation(false);
	
	Spell::load(bean, mob, target);
	m_mob->getMovingBehavior()->setGravityScale(m_gravityScale);
	loadParticleSystem();
}

sf::Vector2f LeapOfFaithSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-40.f, -26.f);
}

void LeapOfFaithSpell::setDisposed() {
	Spell::setDisposed();
	m_mob->getMovingBehavior()->setGravityScale(1.f);
	m_lightObject->setDisposed();
}

void LeapOfFaithSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

void LeapOfFaithSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void LeapOfFaithSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	m_lightObject->setPosition(pos);
	updateParticleSystemPosition();
}

void LeapOfFaithSpell::render(sf::RenderTarget& target) {
	m_ps->render(target);
	Spell::render(target);
}

void LeapOfFaithSpell::update(const sf::Time& frameTime) {
	if (m_isFacingRight != m_mob->isFacingRight()) {
		m_isFacingRight = m_mob->isFacingRight();
		setCurrentAnimation(getAnimation(GameObjectState::Idle), !m_isFacingRight);
	}

	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition);
	setPosition(nextPosition);

	MovableGameObject::update(frameTime);

	GameObject::updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
	m_ps->update(frameTime);
}

void LeapOfFaithSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(100, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob2)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(20.f, 20.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 5.f;
	sizeGen->minEndSize = 0.5f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 204, 150);
	colGen->maxStartCol = sf::Color(255, 255, 255, 250);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 170.f;
	velGen->maxAngle = 190.f;
	velGen->minStartVel = 20.f;
	velGen->maxStartVel = 30.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void LeapOfFaithSpell::updateParticleSystemPosition() {
	if (m_pointGenerator == nullptr) return;
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

