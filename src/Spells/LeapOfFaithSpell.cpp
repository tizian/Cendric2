#include "Spells/LeapOfFaithSpell.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Screens/LevelScreen.h"
#include "GlobalResource.h"

LeapOfFaithSpell::LeapOfFaithSpell(float gravityScale) : Spell() {
	m_gravityScale = gravityScale;
}

void LeapOfFaithSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	m_mob = mob;
	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 80, 120));

	addAnimation(GameObjectState::Idle, spellAnimation);

	m_isFacingRight = m_mob->isFacingRight();
	m_isUpsideDown = false;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), !m_isFacingRight);
	playCurrentAnimation(false);
	
	Spell::load(bean, mob, target);
	m_mob->getMovingBehavior()->setGravityScale(m_gravityScale);
	loadComponents();
}

void LeapOfFaithSpell::setDisposed() {
	Spell::setDisposed();
	m_mob->getMovingBehavior()->setGravityScale(1.f);
	if (m_mob->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
		dynamic_cast<LevelScreen*>(m_screen)->removeTypedBuffs(SpellID::LeapOfFaith);
	}
}

void LeapOfFaithSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

void LeapOfFaithSpell::render(sf::RenderTarget& target) {
	GameObject::render(target);
	target.draw(m_animatedSprite);
}

void LeapOfFaithSpell::update(const sf::Time& frameTime) {
	if (m_mob == nullptr) return;
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
}

void LeapOfFaithSpell::loadComponents() {
	// light
	LightData lightData(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.5f),
		sf::Vector2f(100.f, 100.f), 0.2f));
	addComponent(new LightComponent(lightData, this));

	// particles
	ParticleComponentData data;
	data.particleCount = 100;
	data.emitRate = 50.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	
	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(40.f, 40.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 4.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 1.f;
	sizeGen->maxEndSize = 4.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 204, 150);
	colGen->maxStartCol = sf::Color(255, 255, 255, 250);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 170.f;
	velGen->maxAngle = 190.f;
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 30.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);
}

