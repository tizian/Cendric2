#include "Level/Enemies/ObserverEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "Level/MOBBehavior/MovingBehaviors/ObserverBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Observer, ObserverEnemy)

const float ObserverEnemy::SPEED_IDLE = 50.f;
const float ObserverEnemy::SPEED_CHASING = 150.f;

ObserverEnemy::ObserverEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isInvincible = true;
	m_isAlwaysUpdate = true;
	m_isImmortal = true;
	m_isHPBarVisible = false;

	m_observedRange = 100.f;
	load();
}

void ObserverEnemy::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);

	if (m_state == GameObjectState::Observing) {
		updateTime(m_chasingTime, frameTime);
		m_scriptedBehavior->updateSpeechBubble(frameTime);
		if (m_chasingTime == sf::Time::Zero) {
			setObserverIdle();
		}
	}
	if (m_state == GameObjectState::Triggered) {
		updateTime(m_waitingTime, frameTime);
		m_scriptedBehavior->updateSpeechBubble(frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			setObserverIdle();
		}
	}
	else if (m_state == GameObjectState::Idle && m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
		if (dist(m_mainChar->getPosition(), getPosition()) <= m_observedRange) {
			setObserverChasing();
		}
	}
	updateParticleSystem(frameTime);
}

void ObserverEnemy::setObserverChasing() {
	m_enemyMovingBehavior->setMaxVelocityYDown(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityYUp(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityX(SPEED_CHASING);
	setState(GameObjectState::Observing);
	m_enemyMovingBehavior->resetMovingTarget();
	setChasing();
}

void ObserverEnemy::setObserverTriggered() {
	m_enemyMovingBehavior->setMaxVelocityYDown(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityYUp(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityX(SPEED_CHASING);
	setState(GameObjectState::Triggered);
	m_enemyMovingBehavior->resetMovingTarget();
	setWaiting();
}

void ObserverEnemy::setObserverIdle() {
	m_movingBehavior->setMaxVelocityYDown(SPEED_IDLE);
	m_movingBehavior->setMaxVelocityYUp(SPEED_IDLE);
	m_movingBehavior->setMaxVelocityX(SPEED_IDLE);
	setState(GameObjectState::Idle);
	m_scriptedBehavior->setCurrentObserverStep();
}

void ObserverEnemy::render(sf::RenderTarget& target) {
	m_ps->render(target);
	Enemy::render(target);
}

void ObserverEnemy::loadAttributes() {
	m_attributes.setHealth(1);
	m_attributes.calculateAttributes();
}

bool ObserverEnemy::notifyStealing(bool isFirstTime) {
	if (m_state == GameObjectState::Idle) {
		return false;
	}

	if (isFirstTime) {
		// warn cendric
		setObserverTriggered();
		m_mainChar->setStunned(sf::seconds(3.f));
		if (m_scriptedBehavior != nullptr) {
			m_scriptedBehavior->say("ObserverWarning", 3);
		}
	}
	else {
		// jail him
		setObserverTriggered();
		m_mainChar->setStunned(sf::seconds(999.f));
		if (m_scriptedBehavior != nullptr) {
			m_scriptedBehavior->say("ObserverTriggered", 3);
		}
	}

	return true;
}

MovingBehavior* ObserverEnemy::createMovingBehavior(bool asAlly) {
	ObserverBehavior* behavior = new ObserverBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(SPEED_IDLE);
	behavior->setMaxVelocityYUp(SPEED_IDLE);
	behavior->setMaxVelocityX(SPEED_IDLE);
	return behavior;
}

AttackingBehavior* ObserverEnemy::createAttackingBehavior(bool asAlly) {
	// observers can't attack
	return nullptr;
}

sf::Time ObserverEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(3.f);
}

sf::Time ObserverEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 10 + 5));
}

void ObserverEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 40.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 0, 50, 40));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* observingAnimation = new Animation();
	observingAnimation->setSpriteSheet(tex);
	observingAnimation->addFrame(sf::IntRect(50, 0, 50, 40));

	addAnimation(GameObjectState::Observing, observingAnimation);

	Animation* triggeredAnimation = new Animation();
	triggeredAnimation->setSpriteSheet(tex);
	triggeredAnimation->addFrame(sf::IntRect(100, 0, 50, 40));

	addAnimation(GameObjectState::Triggered, triggeredAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);

	loadParticleSystem();

	LightComponent* lightComponent = new LightComponent(LightData(
		sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), m_observedRange * 2, 1.0f), this);
	addComponent(lightComponent);
}

void ObserverEnemy::updateParticleSystem(const sf::Time& frameTime) {
	m_particleSpawner->center.x = getPosition().x + 0.5f * getBoundingBox()->width;
	m_particleSpawner->center.y = getPosition().y + 0.5f * getBoundingBox()->height;

	m_colGen->minStartCol = m_state == GameObjectState::Idle ?
		sf::Color(40, 40, 200, 200) : m_state == GameObjectState::Observing ?
		sf::Color(200, 100, 50, 200) :
		sf::Color(255, 50, 50, 200);

	m_colGen->maxStartCol = m_state == GameObjectState::Idle ?
		sf::Color(100, 100, 255, 200) : m_state == GameObjectState::Observing ?
		sf::Color(250, 150, 100, 200) :
		sf::Color(255, 100, 100, 200);

	m_colGen->maxEndCol = m_state == GameObjectState::Idle ?
		sf::Color(100, 100, 200, 0) : m_state == GameObjectState::Observing ?
		sf::Color(150, 100, 50, 0) :
		sf::Color(200, 100, 100, 0);

	m_ps->update(frameTime);
}

void ObserverEnemy::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(200, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 80.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::CircleSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->radius = sf::Vector2f(m_observedRange / 2.f, m_observedRange / 2.f);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	m_colGen = m_ps->addGenerator<particles::ColorGenerator>();
	m_colGen->minStartCol = sf::Color(40, 40, 200, 200);
	m_colGen->maxStartCol = sf::Color(200, 100, 50, 200);
	m_colGen->minEndCol = sf::Color(0, 0, 0, 0);
	m_colGen->maxEndCol = sf::Color(200, 100, 100, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 1.f;
	velGen->maxStartSpeed = 2.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

std::string ObserverEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_observer.png";
}

