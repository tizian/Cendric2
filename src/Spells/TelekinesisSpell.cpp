#include "Spells/TelekinesisSpell.h"

TelekinesisSpell::TelekinesisSpell() : Spell() {
}

void TelekinesisSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation(sf::seconds(10.f));
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_telekinesis));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void TelekinesisSpell::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
	Spell::update(frameTime);
	updateParticleSystemPosition();
	checkCollisionsWithItems();
}

void TelekinesisSpell::checkCollisionsWithItems() {
	for (auto& it : *m_items) {
		if (!it->isViewable()) continue;
		if (it->getBoundingBox()->intersects(*getBoundingBox())) {
			LevelItem* item = dynamic_cast<LevelItem*>(it);
			if (item != nullptr) {
				item->pickup();
				setDisposed();
			}
		}
	}
}

void TelekinesisSpell::setItemVector(const std::vector<GameObject*>* items) {
	m_items = items;
}

void TelekinesisSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void TelekinesisSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(50, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob2)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f / 2.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, 0.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 1.f;
	sizeGen->maxEndSize = 3.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(31, 86, 28, 100);
	colGen->maxStartCol = sf::Color(44, 132, 57, 150);
	colGen->minEndCol = sf::Color(0, 255, 0, 0);
	colGen->maxEndCol = sf::Color(200, 255, 200, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 5.f;
	velGen->maxStartVel = 10.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.3f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void TelekinesisSpell::updateParticleSystemPosition() {
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}