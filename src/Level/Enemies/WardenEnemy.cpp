#include "Level/Enemies/WardenEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Level/MOBBehavior/MovingBehaviors/WardenBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/MOBBehavior/AttackingBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Warden, WardenEnemy)

const float WardenEnemy::SPEED_IDLE = 50.f;
const float WardenEnemy::SPEED_CHASING = 150.f;

WardenEnemy::WardenEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isInvincible = true;
	m_isAlwaysUpdate = true;
	m_isImmortal = true;
	m_isHPBarVisible = false;
}

void WardenEnemy::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);

	if (m_wardenState == WardenState::Idle && m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
		if (isMainCharInRange()) {
			m_scriptedBehavior->say("WardenTriggered", 3);
			m_mainChar->setDead();
			m_wardenState = WardenState::Triggered;
		}
	}
}

bool WardenEnemy::isMainCharInRange() {
	if (m_observedRange == 0) return false;

	sf::Vector2f circleDistance;
	auto const& bb = *m_mainChar->getBoundingBox();
	auto const& circle = m_circleSpawner->center;

	circleDistance.x = std::abs(circle.x - bb.left);
	circleDistance.y = abs(circle.y - bb.top);

	if (circleDistance.x > (bb.width * 0.5f + m_observedRange))
		return false;
	if (circleDistance.y > (bb.height * 0.5f + m_observedRange))
		return false;

	if (circleDistance.x <= bb.width * 0.5f)
		return true;
	if (circleDistance.y <= bb.height * 0.5f)
		return true;

	float a = circleDistance.x - bb.width * 0.5f;
	float b = circleDistance.y - bb.height * 0.5f;

	return (a * a + b * b <= m_observedRange * m_observedRange);
}

void WardenEnemy::loadAttributes() {
	m_attributes.setHealth(1);
	m_attributes.calculateAttributes();
}

MovingBehavior* WardenEnemy::createMovingBehavior(bool asAlly) {
	WardenBehavior* behavior = new WardenBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(SPEED_IDLE);
	behavior->setMaxVelocityYUp(SPEED_IDLE);
	behavior->setMaxVelocityX(SPEED_IDLE);
	return behavior;
}

AttackingBehavior* WardenEnemy::createAttackingBehavior(bool asAlly) {
	// wardens can't attack
	return nullptr;
}

void WardenEnemy::updateObservedRange() {
	m_observedRange = (4 - m_mainChar->getInvisibilityLevel()) * 20.f;
	m_circleSpawner->radius = sf::Vector2f(m_observedRange, m_observedRange);
	m_pc->setEmitRate(m_observedRange * 1.5f);
}

void WardenEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	// Idle animation
	Animation* idleAnimation = new Animation(sf::milliseconds(150));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	idleAnimation->addFrame(sf::IntRect(0 * 50, 0, 50, 50));
	idleAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	idleAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 50));
	addAnimation(GameObjectState::Idle, idleAnimation);

	// Blinking animation
	Animation* blinkingAnimation = new Animation(sf::milliseconds(150));
	blinkingAnimation->setSpriteSheet(tex);
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(10 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(11 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(11 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(10 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 50));
	addAnimation(GameObjectState::Blinking, blinkingAnimation);

	// Looking Animation
	Animation* lookingAnimation = new Animation(sf::milliseconds(150));
	lookingAnimation->setSpriteSheet(tex);
	lookingAnimation->addFrame(sf::IntRect(3 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(5 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(3 * 50, 0, 50, 50));

	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(0 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));

	lookingAnimation->addFrame(sf::IntRect(6 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(8 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(6 * 50, 0, 50, 50));
	addAnimation(GameObjectState::Looking, lookingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadComponents();
}

void WardenEnemy::loadComponents() {
	// light
	LightComponent* lightComponent = new LightComponent(LightData(
		sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 250.f, 0.8f), this);
	addComponent(lightComponent);

	// particles
	ParticleComponentData data;
	data.particleCount = 300;
	data.isAdditiveBlendMode = true;
	data.emitRate = m_observedRange;
	data.texturePath = getParticleTexture();

	// Generators
	m_circleSpawner = new particles::CircleSpawner();
	m_circleSpawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	m_circleSpawner->radius = sf::Vector2f(m_observedRange, m_observedRange);
	data.spawner = m_circleSpawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 15.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	m_colGen = new particles::ColorGenerator();
	m_colGen->minStartCol = sf::Color(200, 40, 40, 200);
	m_colGen->maxStartCol = sf::Color(200, 100, 50, 200);
	m_colGen->minEndCol = sf::Color(0, 0, 0, 0);
	m_colGen->maxEndCol = sf::Color(200, 100, 100, 0);
	data.colorGen = m_colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 1.f;
	velGen->maxStartSpeed = 2.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_pc);

	updateObservedRange();
}

std::string WardenEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_warden.png";
}

std::string WardenEnemy::getParticleTexture() const {
	return GlobalResource::TEX_PARTICLE_FLAME;
}
