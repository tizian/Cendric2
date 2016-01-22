#include "Level/Enemies/NekomataEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/EnemyBehavior/WalkingBehavior.h"
#include "Level/EnemyBehavior/AggressiveBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Nekomata_blue, NekomataEnemy)

void NekomataEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	loot.insert({ "fo_rawmeat", 1 });
	gold = rand() % 10 + 2;
}

NekomataEnemy::NekomataEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Nekomata_blue);
}

void NekomataEnemy::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.resistanceLight = -30;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Shadow);
}

void NekomataEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 100;
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f NekomataEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(30.f, 20.f);
}

MovingBehavior* NekomataEnemy::createMovingBehavior() {
	WalkingBehavior* behavior = new WalkingBehavior(this);
	behavior->setJumpHeight(getConfiguredMaxVelocityYUp() * getConfiguredMaxVelocityYUp() / (2 * getConfiguredGravityAcceleration()));
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(30.f);
	return behavior;
}

AttackingBehavior* NekomataEnemy::createAttackingBehavior() {
	AggressiveBehavior* behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&NekomataEnemy::handleAttackInput, this));
	return behavior;
}

void NekomataEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_attackingBehavior->distToTarget() < m_attackingBehavior->getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (m_attackingBehavior->distToTarget() < 150.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void NekomataEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 130.f, 70.f));
	setSpriteOffset(sf::Vector2f(-10.f, -11.f));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));

	for (int i = 0; i < 12; i++) {
		walkingAnimation.addFrame(sf::IntRect(0, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		jumpingAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation jumpingStartAnimation(sf::seconds(0.08f));
	jumpingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 1; i < 4; i++) {
		jumpingStartAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping_start, jumpingStartAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		idleAnimation.addFrame(sf::IntRect(300, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation fightingStartAnimation(sf::seconds(0.08f));
	fightingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		fightingStartAnimation.addFrame(sf::IntRect(450, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Fighting_start, fightingStartAnimation);

	Animation fightingAnimation(sf::seconds(0.08f));
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	fightingAnimation.addFrame(sf::IntRect(450, 1 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 3 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) {
		deadAnimation.addFrame(sf::IntRect(600, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Dead, deadAnimation);

	m_jumpStartTime = sf::seconds(3 * 0.08f);
	m_fightStartTime = sf::seconds(1 * 0.08f);

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

float NekomataEnemy::getMaxVelocityYUp() const {
	return 600.f;
}

float NekomataEnemy::getMaxVelocityYDown() const {
	return 600.f;
}

float NekomataEnemy::getMaxVelocityX() const {
	return 80.f;
}

sf::Time NekomataEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(6 * 80);
}

int NekomataEnemy::getMentalStrength() const {
	return 3;
}

void NekomataEnemy::updateAnimation(const sf::Time& frameTime) {
	// a nekomata has additional states

	GameObjectState newState = GameObjectState::Idle;
	if (m_isDead) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		if (m_state == GameObjectState::Fighting) {
			newState = GameObjectState::Fighting;
		}
		else {
			if (m_currentFightStartTime == sf::Time::Zero) {
				m_currentFightStartTime = m_fightStartTime;
				newState = GameObjectState::Fighting_start;
			}
			else {
				updateTime(m_currentFightStartTime, frameTime);
				if (m_currentFightStartTime == sf::Time::Zero) {
					newState = GameObjectState::Fighting;
				}
				else {
					newState = GameObjectState::Fighting_start;
				}
			}
		}
	}
	else if (!m_isGrounded) {
		if (m_state == GameObjectState::Jumping) {
			newState = GameObjectState::Jumping;
		}
		else {
			if (m_currentJumpStartTime == sf::Time::Zero) {
				m_currentJumpStartTime = m_jumpStartTime;
				newState = GameObjectState::Jumping_start;
			}
			else {
				updateTime(m_currentJumpStartTime, frameTime);
				if (m_currentJumpStartTime == sf::Time::Zero) {
					newState = GameObjectState::Jumping;
				}
				else {
					newState = GameObjectState::Jumping_start;
				}
			}
		}
	}
	else if (std::abs(getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_state != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
}


