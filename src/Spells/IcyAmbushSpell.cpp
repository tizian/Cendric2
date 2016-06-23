#include "Spells/IcyAmbushSpell.h"
#include "GlobalResource.h"

IcyAmbushSpell::~IcyAmbushSpell() {
	delete m_ps;
}

void IcyAmbushSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation(sf::seconds(10.f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 40, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void IcyAmbushSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();
}

void IcyAmbushSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void IcyAmbushSpell::execOnHit(LevelMovableGameObject *target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() < m_data.ccStrength) {
			enemy->setStunned(m_data.duration);
		}
	}
	// main character can't be stunned yet.

	// check if port of owner is possible
	WorldCollisionQueryRecord rec;
	rec.boundingBox = *(m_mob->getBoundingBox());
	rec.boundingBox.left = target->getBoundingBox()->left + target->getBoundingBox()->width / 2.f - rec.boundingBox.width / 2.f;
	rec.boundingBox.top = target->getBoundingBox()->top + (target->getBoundingBox()->height - rec.boundingBox.height);
	if (!m_level->collides(rec)) {
		m_mob->setPosition(sf::Vector2f(rec.boundingBox.left, rec.boundingBox.top));
	}
	else {
		g_logger->logInfo("IcyAmbushSpell", "Icy ambush port would stuck its owner. The port is not executed.");
	}
	setDisposed();
}

void IcyAmbushSpell::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(500, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_SNOWFLAKE));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 500.0f / 5.0f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::BoxSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 4.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 200.f;
	velGen->maxStartSpeed = 200.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.5f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void IcyAmbushSpell::updateParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
}