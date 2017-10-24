#include "Spells/TelekinesisSpell.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"

TelekinesisSpell::TelekinesisSpell() : Spell() {
}

void TelekinesisSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation(sf::seconds(10.f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 40, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	loadComponents();
}

void TelekinesisSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;

	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);

	// check collisions with dynamic tiles
	if (m_data.isDynamicTileEffect) {
		sf::FloatRect tmp(nextPosition, sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height));
		m_level->collideWithDynamicTiles(this, tmp);
	}

	checkCollisionsWithItems();

	MovableGameObject::update(frameTime);

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
}

void TelekinesisSpell::checkCollisionsWithItems() {
	for (auto& it : *m_items) {
		if (!it->isViewable()) continue;
		if (fastIntersect(*it->getBoundingBox(), *getBoundingBox())) {
			LevelItem* item = dynamic_cast<LevelItem*>(it);
			if (item != nullptr) {
				if (m_mob->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
					item->pickup();
				}
				else {
					item->setDisposed();
				}
				setDisposed();
				break;
			}
		}
	}
}

void TelekinesisSpell::setItemVector(const std::vector<GameObject*>* items) {
	m_items = items;
}

void TelekinesisSpell::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 50;
	data.emitRate = 25.f;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	data.isAdditiveBlendMode = true;

	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 2.f;
	sizeGen->maxEndSize = 6.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(31, 86, 28, 100);
	colGen->maxStartCol = sf::Color(44, 132, 57, 150);
	colGen->minEndCol = sf::Color(0, 255, 0, 0);
	colGen->maxEndCol = sf::Color(200, 255, 200, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 5.f;
	velGen->maxStartSpeed = 10.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.3f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);
}
