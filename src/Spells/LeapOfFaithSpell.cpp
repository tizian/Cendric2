#include "Spells/LeapOfFaithSpell.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "GameObjectComponents/LightComponent.h"
#include "GlobalResource.h"

LeapOfFaithSpell::LeapOfFaithSpell(float gravityScale) : Spell() {
	m_gravityScale = gravityScale;
}

LeapOfFaithSpell::~LeapOfFaithSpell() {
	delete m_ps;
}

void LeapOfFaithSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	m_mob = mob;
	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 80, 120));

	addAnimation(GameObjectState::Idle, spellAnimation);

	m_isFacingRight = m_mob->isFacingRight();
	m_isUpsideDown = m_mob->isUpsideDown();
	setCurrentAnimation(getAnimation(GameObjectState::Idle), !m_isFacingRight);
	playCurrentAnimation(false);
	
	Spell::load(bean, mob, target);
	m_mob->getMovingBehavior()->setGravityScale(m_gravityScale);
	loadParticleSystem();

	LightData lightData(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.5f), 
		sf::Vector2f(100.f, 100.f), 0.2f));
	addComponent(new LightComponent(lightData, this));
}

void LeapOfFaithSpell::setDisposed() {
	Spell::setDisposed();
	m_mob->getMovingBehavior()->setGravityScale(1.f);
}

void LeapOfFaithSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

void LeapOfFaithSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
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

	if (m_isUpsideDown != m_mob->isUpsideDown()) {
		m_isUpsideDown = m_mob->isUpsideDown();
		m_animatedSprite.setFlippedY(m_isUpsideDown);
	}

	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);

	MovableGameObject::update(frameTime);

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
	m_ps->update(frameTime);
}

void LeapOfFaithSpell::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(100, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::BoxSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(40.f, 40.f);
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 4.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 1.f;
	sizeGen->maxEndSize = 4.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 204, 150);
	colGen->maxStartCol = sf::Color(255, 255, 255, 250);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 170.f;
	velGen->maxAngle = 190.f;
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 30.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void LeapOfFaithSpell::updateParticleSystemPosition() {
	if (m_particleSpawner == nullptr) return;
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
}

