#include "Spells/GhostFormSpell.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/LevelEquipment.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "GlobalResource.h"

const sf::Color GhostFormSpell::GHOST_COLOR = sf::Color(100, 200, 255, 150);

GhostFormSpell::GhostFormSpell(const AttributeData& additionalDamage) : Spell() {
	m_additionalDamage = additionalDamage;
}

void GhostFormSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	MovingBehavior* mb = mob->getMovingBehavior();
	float velocityScale = (bean.speed + mb->getMaxVelocityX()) / mb->getMaxVelocityX();
	SpellData data(bean);
	data.speed = 0.f;
	data.boundingBox = *mob->getBoundingBox();
	Spell::load(data, mob, target);
	loadParticleSystem(bean.speed);

	mb->setMaxXVelocityScale(velocityScale);
	mb->setIgnoreDynamicTiles(true);
	m_mob->addAttributes(getActiveDuration(), m_additionalDamage);
	loadMask();
}

void GhostFormSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition);
	setPosition(nextPosition);
	m_mob->setSpriteColor(GHOST_COLOR, sf::milliseconds(100));

	MovableGameObject::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration <= sf::Time::Zero) {
		setDisposed();
	}
}

void GhostFormSpell::setDisposed() {
	Spell::setDisposed();
	if (m_mask != nullptr) {
		m_mask->setDisposed();
	}
		
	MovingBehavior* mb = m_mob->getMovingBehavior();
	mb->setMaxXVelocityScale(1.f);
	mb->setIgnoreDynamicTiles(false);

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *m_mob->getBoundingBox();
	if (m_level->collides(rec)) {
		m_mob->setDead();
	}
}

void GhostFormSpell::render(sf::RenderTarget& target) {
	m_ps->render(target);
}

void GhostFormSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

bool GhostFormSpell::getConfiguredRotateSprite() const {
	return false;
}

void GhostFormSpell::loadParticleSystem(float startVelocity) {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(100, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_LONGBLOB)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(1.f, m_mob->getBoundingBox()->height / 2.f);
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 5.f;
	sizeGen->minEndSize = 0.5f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(89, 222, 100, 200);
	colGen->maxStartCol = sf::Color(157, 254, 167, 250);
	colGen->minEndCol = sf::Color(200, 255, 200, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minStartVel = startVelocity / 2.f - 10.f;
	velGen->maxStartVel = startVelocity / 2.f + 10.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void GhostFormSpell::updateParticleSystemPosition() {
	if (m_mob->isFacingRight()) {
		m_velGenerator->minAngle = 260.f;
		m_velGenerator->maxAngle = 280.f;
	}
	else {
		m_velGenerator->minAngle = 80.f;
		m_velGenerator->maxAngle = 100.f;
	}
	m_posGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_posGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

void GhostFormSpell::loadMask() {
	if (m_mob->getConfiguredType() != GameObjectType::_LevelMainCharacter || m_mask != nullptr) return;
	
	std::map<GameObjectState, std::vector<sf::IntRect>> texturePositions;
	for (int i = 0; i < 8; i++) {
		texturePositions[GameObjectState::Walking].push_back(sf::IntRect(i * 120, 0, 120, 120));
	}
	for (int i = 0; i < 2; i++) {
		texturePositions[GameObjectState::Idle].push_back(sf::IntRect(960 + i * 120, 0, 120, 120));
	}
	for (int i = 0; i < 2; i++) {
		texturePositions[GameObjectState::Jumping].push_back(sf::IntRect(1200 + i * 120, 0, 120, 120));
	}
	for (int i = 0; i < 5; i++) {
		texturePositions[GameObjectState::Fighting].push_back(sf::IntRect(1440 + i * 120, 0, 120, 120));
	}

	LevelEquipment* levelEquipment = new LevelEquipment(m_mainChar);
	levelEquipment->setBoundingBox(sf::FloatRect(0, 0, 120, 120));
	for (auto& ani : texturePositions) {
		Animation* animation = new Animation();
		if (ani.first == GameObjectState::Fighting) {
			animation->setFrameTime(sf::milliseconds(70));
		}
		else if (ani.first == GameObjectState::Jumping) {
			animation->setFrameTime(sf::milliseconds(200));
		}
		animation->setSpriteSheet(g_resourceManager->getTexture(m_data.spritesheetPath));
		for (auto& frame : ani.second) {
			animation->addFrame(frame);
		}
		levelEquipment->addAnimation(ani.first, animation);
	}

	// initial values
	levelEquipment->setCurrentAnimation(levelEquipment->getAnimation(GameObjectState::Idle), false);
	levelEquipment->playCurrentAnimation(true);
	levelEquipment->loadEquipment();
	levelEquipment->setCopyingMainCharColor(false);
	m_mask = levelEquipment;
	m_screen->addObject(levelEquipment);
}