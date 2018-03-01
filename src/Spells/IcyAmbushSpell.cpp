#include "Spells/IcyAmbushSpell.h"
#include "GlobalResource.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"

void IcyAmbushSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation(sf::seconds(10.f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 30 * bean.skinNr, 40, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	loadComponents();
}

void IcyAmbushSpell::execOnHit(LevelMovableGameObject* target) {
	Spell::execOnHit(target);
	if (!m_mob) return;

	// check if port of owner is possible
	WorldCollisionQueryRecord rec;
	rec.boundingBox = *(m_mob->getBoundingBox());
	rec.boundingBox.left = target->getBoundingBox()->left + target->getBoundingBox()->width / 2.f - rec.boundingBox.width / 2.f;
	rec.boundingBox.top = target->getBoundingBox()->top + (target->getBoundingBox()->height - rec.boundingBox.height);
	if (!m_level->collides(rec)) {
		m_mob->setPosition(sf::Vector2f(rec.boundingBox.left, rec.boundingBox.top));
	}
	else {
		m_screen->setNegativeTooltip("NotTeleport");
		g_logger->logInfo("IcyAmbushSpell", "Icy ambush port would stuck its owner. The port is not executed.");
	}
}

void IcyAmbushSpell::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 500;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_SNOWFLAKE;

	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 4.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 200.f;
	velGen->maxStartSpeed = 200.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.5f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);

	LightData lightData(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 50.f, 0.5f);
	addComponent(new LightComponent(lightData, this));
}
