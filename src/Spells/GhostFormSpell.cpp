#include "Spells/GhostFormSpell.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/LevelEquipment.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "GameObjectComponents/ParticleComponent.h"
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
	loadComponents(bean.speed);

	mb->setMaxXVelocityScale(velocityScale);
	mb->setIgnoreDynamicTiles(true);
	m_mob->addAttributes(getActiveDuration(), m_additionalDamage);
	loadMask();
}

void GhostFormSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);
	m_mob->setSpriteColor(GHOST_COLOR, sf::milliseconds(100));

	MovableGameObject::update(frameTime);
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
	GameObject::render(target);
}

void GhostFormSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

bool GhostFormSpell::getConfiguredRotateSprite() const {
	return false;
}

void GhostFormSpell::loadComponents(float startSpeed) {
	ParticleComponentData data;
	data.isAdditiveBlendMode = true;
	data.emitRate = 50.f;
	data.particleCount = 100;
	data.texturePath = GlobalResource::TEX_PARTICLE_LONGBLOB;

	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(2.f, m_mob->getBoundingBox()->height);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(89, 222, 100, 200);
	colGen->maxStartCol = sf::Color(157, 254, 167, 250);
	colGen->minEndCol = sf::Color(200, 255, 200, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	m_velGen = new particles::AngledVelocityGenerator();
	m_velGen->minStartSpeed = startSpeed / 2.f - 10.f;
	m_velGen->maxStartSpeed = startSpeed / 2.f + 10.f;
	data.velGen = m_velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	pc->getParticleSystem()->addGenerator<particles::DirectionDefinedRotationGenerator>();
	addComponent(pc);
}

void GhostFormSpell::updateParticleSystemPosition() {
	if (m_mob->isFacingRight()) {
		m_velGen->minAngle = 260.f;
		m_velGen->maxAngle = 280.f;
	}
	else {
		m_velGen->minAngle = 80.f;
		m_velGen->maxAngle = 100.f;
	}
}

void GhostFormSpell::loadMask() {
	if (m_mob->getConfiguredType() != GameObjectType::_LevelMainCharacter || m_mask != nullptr) return;

	m_mask = new LevelEquipment(m_mainChar);
	ItemEquipmentBean itemBean;
	itemBean.texture_path = m_data.spritesheetPath;
	itemBean.frames_walk = 8;
	itemBean.frames_idle = 1;
	itemBean.frames_jump = 1;
	itemBean.frames_fight = 5;
	itemBean.frames_climb1 = 1;
	itemBean.frames_climb2 = 1;
	itemBean.status = BeanStatus::Filled;

	m_mask->load(&itemBean, nullptr, ItemType::VOID);
	
	m_mask->setCopyingMainCharColor(false);
	m_screen->addObject(m_mask);
}