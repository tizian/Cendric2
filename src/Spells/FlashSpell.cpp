#include "Spells/FlashSpell.h"

FlashSpell::FlashSpell() : Spell() {
}

void FlashSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, 0.f));
	m_mob = mob;
	
	Animation spellAnimation(sf::milliseconds(200));
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_flash));
	spellAnimation.addFrame(sf::IntRect(0, 0, 120, 120));
	spellAnimation.addFrame(sf::IntRect(120, 0, 120, 120));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
	
	Spell::load(data, mob, target);
	m_isFlashingRight = mob->isFacingRight();
	
	m_flashingTime = FLASHING_TIME;
	m_flashDuration = FLASH_DURATION;

	sf::Vector2f position(m_mob->getPosition());
	position.x += (m_mob->getBoundingBox()->width / 2.f);
	position.y += m_mob->getBoundingBox()->height - 20;
	position.x = m_isFlashingRight ? position.x - data.range : position.x + data.range;
	loadParticleSystem();
	m_posGenerator->center = position;
}

void FlashSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	GameObject::updateTime(m_flashingTime, frameTime);
	if (m_flashDuration > sf::Time::Zero) {
		m_flashDuration -= frameTime;
		if (m_flashDuration <= sf::Time::Zero) {
			m_data.damageType = DamageType::VOID;
			m_isHurting = false;
		}
	}
	if (m_flashingTime == sf::Time::Zero) {
		m_isVisible = !m_isVisible;
		m_flashingTime = FLASHING_TIME;
	}
	m_ps->update(frameTime);
	if (m_ps->emitRate > 0 && m_data.activeDuration < sf::seconds(1.f)) {
		m_ps->emitRate = 0;
	}
}

void FlashSpell::render(sf::RenderTarget& target) {
	if (m_isVisible && m_flashDuration > sf::Time::Zero) {
		Spell::render(target);
	}
	m_ps->render(target);
}

void FlashSpell::execOnHit(LevelMovableGameObject* target) {
	// this spell only hurts once
	if (m_isHurting) {
		m_isHurting = false;
	}
	else {
		m_data.damageType = DamageType::VOID;
	}
}

bool FlashSpell::getConfiguredRotateSprite() const {
	return false;
}

sf::Vector2f FlashSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-50.f, -20.f);
}

void FlashSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(50, g_resourceManager->getTexture(ResourceID::Texture_Particle_smoke)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::DiskPositionGenerator>();
	posGen->radius = 20.f;
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 40.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 60.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(24, 21, 57, 100);
	colGen->maxStartCol = sf::Color(51, 51, 71, 150);
	colGen->minEndCol = sf::Color(166, 167, 198, 0);
	colGen->maxEndCol = sf::Color(198, 199, 210, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -30.f;
	velGen->maxAngle = 30.f;
	velGen->minStartVel = 70.f;
	velGen->maxStartVel = 100.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 1.2f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}
