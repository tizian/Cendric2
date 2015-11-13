#include "Spells/IcyAmbushSpell.h"

IcyAmbushSpell::IcyAmbushSpell(const sf::Time& stunDuration) : Spell() {
	m_stunDuration = stunDuration;
}

void IcyAmbushSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation(sf::seconds(10.f));
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_icyambush));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void IcyAmbushSpell::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
	if (!m_isStunning) {
		Spell::update(frameTime);
		updateParticleSystemPosition();
	}
	else {
		GameObject::updateTime(m_duration, frameTime);
		if (m_duration <= sf::Time::Zero) setDisposed();
	}
}

void IcyAmbushSpell::render(sf::RenderTarget& target) {
	if (!m_isStunning) {
		Spell::render(target);
	}
	m_ps->render(target);
}

void IcyAmbushSpell::execOnHit(LevelMovableGameObject *target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() <= 2) {
			enemy->setStunned(m_stunDuration);
		}
	}
	// main character can't be stunned yet.
	m_isDisposed = false;
	if (!m_isStunning) {
		m_isStunning = true;
		m_duration = m_stunDuration;
		m_damage = 0;
		m_boundingBox.width = m_range * 2;
		m_boundingBox.height = m_range * 2;
		setPosition(getPosition() + (getPosition() - getCenter()));

		m_debugBox.setPosition(getPosition());
		m_debugBox.setSize(sf::Vector2f(m_boundingBox.width, m_boundingBox.height));

		updateParticleSystemPosition();

		if (m_ownerType != GameObjectType::_MainCharacter) {
			checkCollisionsWithMainChar(getBoundingBox());
		}
		// check collisions with enemies
		checkCollisionsWithEnemies(getBoundingBox());

		// check if port of owner is possible
		sf::FloatRect ownerBB = *(m_mob->getBoundingBox());
		ownerBB.left = target->getBoundingBox()->left + target->getBoundingBox()->width / 2.f - ownerBB.width / 2.f;
		ownerBB.top = target->getBoundingBox()->top + (target->getBoundingBox()->height - ownerBB.height);
		if (!m_level->collidesX(ownerBB) && !m_level->collidesY(ownerBB)) {
			m_mob->setPosition(sf::Vector2f(ownerBB.left, ownerBB.top));
		}
		else {
			g_logger->logInfo("IcyAmbushSpell", "Icy ambush port would stuck its owner. The port is not executed.");
		}
	}
}

void IcyAmbushSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(500, g_resourceManager->getTexture(ResourceID::Texture_Particle_snowflake)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 500.0f / 5.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, 0.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 6.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 200.f;
	velGen->maxStartVel = 200.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = 1.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void IcyAmbushSpell::updateParticleSystemPosition() {
	if (!m_isStunning) {
		m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
		m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
}