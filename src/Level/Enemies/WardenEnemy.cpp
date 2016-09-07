#include "Level/Enemies/WardenEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
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

	m_observedRange = 100.f;
}

void WardenEnemy::update(const sf::Time& frameTime) {
	LevelMovableGameObject::update(frameTime);

	if (m_state == GameObjectState::Idle && m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
		if (AttackingBehavior::isInAggroRange(m_mainChar, this, m_observedRange)) {
			m_scriptedBehavior->say("WardenTriggered", 3);
			m_mainChar->setDead();
			m_wardenState = WardenState::Triggered;
		}
	}

	updateParticleSystem(frameTime);
}

void WardenEnemy::render(sf::RenderTarget& target) {
	m_ps->render(target);
	Enemy::render(target);
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

void WardenEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	// Idle animation
	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 0, 50, 50));
	addAnimation(GameObjectState::Idle, idleAnimation);

	// Blinking animation
	Animation* blinkingAnimation = new Animation();
	blinkingAnimation->setSpriteSheet(tex);
	blinkingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(8 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(9 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(8 * 50, 0, 50, 50));
	blinkingAnimation->addFrame(sf::IntRect(7 * 50, 0, 50, 50));
	addAnimation(GameObjectState::Blinking, blinkingAnimation);

	// Looking Animation
	Animation* lookingAnimation = new Animation();
	lookingAnimation->setSpriteSheet(tex);
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(3 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(2 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(1 * 50, 0, 50, 50));
	for (int i = 0; i < 4; ++i) {
		lookingAnimation->addFrame(sf::IntRect(0 * 50, 0, 50, 40));
	}
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(5 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(6 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(5 * 50, 0, 50, 50));
	lookingAnimation->addFrame(sf::IntRect(4 * 50, 0, 50, 50));
	addAnimation(GameObjectState::Looking, lookingAnimation);
	
	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadParticleSystem();

	LightComponent* lightComponent = new LightComponent(LightData(
		sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), m_observedRange * 2, 1.0f), this);
	addComponent(lightComponent);
}

void WardenEnemy::updateParticleSystem(const sf::Time& frameTime) {
	m_particleSpawner->center.x = getPosition().x + 0.5f * getBoundingBox()->width;
	m_particleSpawner->center.y = getPosition().y + 0.5f * getBoundingBox()->height;

	m_ps->update(frameTime);
}

void WardenEnemy::loadParticleSystem() {
	delete m_ps;
	m_ps = new particles::TextureParticleSystem(200, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
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
	m_colGen->minStartCol = sf::Color(200, 40, 40, 200);
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

std::string WardenEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_warden.png";
}

