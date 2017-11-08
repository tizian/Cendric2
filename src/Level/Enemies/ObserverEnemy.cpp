#include "Level/Enemies/ObserverEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "Level/MOBBehavior/MovingBehaviors/WardenBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/MOBBehavior/AttackingBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Observer, ObserverEnemy)

ObserverEnemy::ObserverEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	WardenEnemy(level, screen) {

	m_isInvincible = true;
	m_isAlwaysUpdate = true;
	m_isImmortal = true;
	m_isHPBarVisible = false;
}

void ObserverEnemy::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);
	
	if (m_wardenState == WardenState::Observing) {
		updateTime(m_chasingTime, frameTime);
		m_scriptedBehavior->updateSpeechBubble(frameTime);
		if (m_chasingTime == sf::Time::Zero) {
			setObserverIdle();
		}
	}
	if (m_wardenState == WardenState::Triggered) {
		updateTime(m_waitingTime, frameTime);
		m_scriptedBehavior->updateSpeechBubble(frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			setObserverIdle();
		}
	}
	else if (m_wardenState == WardenState::Idle && m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
		if (isMainCharInRange()) {
			setObserverChasing();
		}
	}
	updateColors();
}

void ObserverEnemy::setObserverChasing() {
	m_enemyMovingBehavior->setMaxVelocityYDown(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityYUp(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityX(SPEED_CHASING);
	m_wardenState = WardenState::Observing;
	setAnimationTextureY(40);
	m_enemyMovingBehavior->resetMovingTarget();
	setChasing();
}

void ObserverEnemy::setObserverTriggered() {
	m_enemyMovingBehavior->setMaxVelocityYDown(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityYUp(SPEED_CHASING);
	m_enemyMovingBehavior->setMaxVelocityX(SPEED_CHASING);
	m_wardenState = WardenState::Triggered;
	setAnimationTextureY(80);
	m_enemyMovingBehavior->resetMovingTarget();
	setWaiting();
}

void ObserverEnemy::setObserverIdle() {
	m_movingBehavior->setMaxVelocityYDown(SPEED_IDLE);
	m_movingBehavior->setMaxVelocityYUp(SPEED_IDLE);
	m_movingBehavior->setMaxVelocityX(SPEED_IDLE);
	setAnimationTextureY(0);
	m_wardenState = WardenState::Idle;
	
	m_scriptedBehavior->setCurrentRoutineStep();
}

bool ObserverEnemy::notifyStealing(bool isFirstTime) {
	if (m_wardenState == WardenState::Idle) {
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

sf::Time ObserverEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(3.f);
}

sf::Time ObserverEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 4));
}

void ObserverEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 40.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	// Idle animation
	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0 , 0, 50, 40));
	addAnimation(GameObjectState::Idle, idleAnimation);

	// Blinking animation
	Animation* blinkingAnimation = new Animation();
	blinkingAnimation->setSpriteSheet(tex);
	blinkingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 40));
	blinkingAnimation->addFrame(sf::IntRect(8 * 50, 0, 50, 40));
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 40));
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 40));
	blinkingAnimation->addFrame(sf::IntRect(8 * 50, 0, 50, 40));
	blinkingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 40));
	addAnimation(GameObjectState::Blinking, blinkingAnimation);

	// Looking Animation
	Animation* lookingAnimation = new Animation();
	lookingAnimation->setSpriteSheet(tex);
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(3 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 40));
	for (int i = 0; i < 4; ++i) {
		lookingAnimation->addFrame(sf::IntRect(0 * 50, 0, 50, 40));
	}
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(5 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(6 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(5 * 50, 0, 50, 40));
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 40));
	addAnimation(GameObjectState::Looking, lookingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadComponents();
}

void ObserverEnemy::setAnimationTextureY(int y) {
	const Animation* idleAnimation = getAnimation(GameObjectState::Idle);
	size_t animationSize = idleAnimation->getSize();
	for (size_t frame = 0; frame < animationSize; ++frame) {
		// oh no, a const cast? shame on me.
		((sf::IntRect&)idleAnimation->getFrame(frame)).top = y;
	}

	const Animation* blinkingAnimation = getAnimation(GameObjectState::Blinking);
	animationSize = blinkingAnimation->getSize();
	for (size_t frame = 0; frame < animationSize; ++frame) {
		((sf::IntRect&)blinkingAnimation->getFrame(frame)).top = y;
	}

	const Animation* lookingAnimation = getAnimation(GameObjectState::Looking);
	animationSize = lookingAnimation->getSize();
	for (size_t frame = 0; frame < animationSize; ++frame) {
		((sf::IntRect&)lookingAnimation->getFrame(frame)).top = y;
	}
}

void ObserverEnemy::updateColors() {
	m_colGen->minStartCol = m_wardenState == WardenState::Idle ?
		sf::Color(40, 40, 200, 200) : m_wardenState == WardenState::Observing ?
		sf::Color(200, 100, 50, 200) :
		sf::Color(255, 50, 50, 200);

	m_colGen->maxStartCol = m_wardenState == WardenState::Idle ?
		sf::Color(100, 100, 255, 200) : m_wardenState == WardenState::Observing ?
		sf::Color(250, 150, 100, 200) :
		sf::Color(255, 100, 100, 200);

	m_colGen->maxEndCol = m_wardenState == WardenState::Idle ?
		sf::Color(100, 100, 200, 0) : m_wardenState == WardenState::Observing ?
		sf::Color(150, 100, 50, 0) :
		sf::Color(200, 100, 100, 0);
}

std::string ObserverEnemy::getParticleTexture() const {
	return GlobalResource::TEX_PARTICLE_STAR;
}

std::string ObserverEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_observer.png";
}
