#include "Level/MOBBehavior/MovingBehaviors/UserMovingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Level/DynamicTiles/LadderTile.h"

const sf::Time UserMovingBehavior::JUMP_GRACE_TIME = sf::milliseconds(100);
const sf::Time UserMovingBehavior::CLIMB_STEP_TIME = sf::milliseconds(200);

UserMovingBehavior::UserMovingBehavior(LevelMainCharacter* mainChar) : MovingBehavior(mainChar) {
	// use this assignment because the "normal" assigner in moving behavior can't get this yet.
	m_mainChar = mainChar;
}

void UserMovingBehavior::update(const sf::Time& frameTime) {
	updateTime(m_jumpGraceTime, frameTime);
	bool wasGrounded = m_isGrounded;
	MovingBehavior::update(frameTime);
	if (wasGrounded && !m_isGrounded) {
		m_jumpGraceTime = JUMP_GRACE_TIME;
	}
	handleClimbing(frameTime);
}

void UserMovingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	bool collidesX;
	bool collidesY;
	MovingBehavior::checkXYDirection(nextPosition, collidesX, collidesY);
}

void UserMovingBehavior::stopClimbing() {
	m_climbStepTime = sf::Time::Zero;
	m_isClimbing = false;
	m_currentLadder = nullptr;
}

void UserMovingBehavior::startClimbing(GameObject* ladder, float yPos) {
	m_isClimbing = true;
	m_isGrounded = true;
	m_currentLadder = ladder;
	m_mob->setPositionY(yPos);
	m_mob->setAccelerationY(0.f);
	m_mob->setVelocityY(0.f);
	g_inputController->lockAction();
}

void UserMovingBehavior::handleClimbing(const sf::Time& frameTime) {
	if (m_isClimbing) {
		if (g_inputController->isKeyJustPressed(Key::Jump)) {
			stopClimbing();
			g_inputController->lockAction();
			return;
		}

		// handle climb step timing
		if (g_inputController->isKeyActive(Key::Up) || g_inputController->isKeyActive(Key::Down)) {
			m_climbStepTime += frameTime;
			if (m_climbStepTime >= CLIMB_STEP_TIME) {
				m_climbStepTime = sf::Time::Zero;
				bool up = g_inputController->isKeyActive(Key::Up);
				float diffY = static_cast<float>(up ? -LadderTile::LADDER_STEP : LadderTile::LADDER_STEP);
				
				WorldCollisionQueryRecord rec;
				rec.collisionDirection = up ? CollisionDirection::Up : CollisionDirection::Down;
				rec.boundingBox = *(m_mob->getBoundingBox());
				rec.boundingBox.top += diffY;
				rec.ignoreDynamicTiles = m_mob->isIgnoreDynamicTiles();
				rec.ignoreOnewayTiles = true;

				if (m_mob->getLevel()->collides(rec)) {
					if (!up != m_mob->isUpsideDown()) {
						stopClimbing();
					}
					return;
				}

				m_mob->setPositionY(rec.boundingBox.top);
				m_isClimbingStep1 = !m_isClimbingStep1;
			}
		}
		else {
			m_climbStepTime = sf::Time::Zero;
		}

		// check if we're still on our ladder
		if (!m_mob->getBoundingBox()->intersects(*m_currentLadder->getBoundingBox())) {
			stopClimbing();
		}
	}
	else {
		// check if a climbing just started
		if (!(g_inputController->isKeyJustPressed(Key::Up) || g_inputController->isKeyJustPressed(Key::Down))) return;

		for (auto& go : *(m_mob->getScreen()->getObjects(GameObjectType::_DynamicTile))) {
			LadderTile* tile = dynamic_cast<LadderTile*>(go);
			if (tile && tile->isViewable() &&
				tile->getBoundingBox()->intersects(*m_mob->getBoundingBox())) {

				float climbingY = tile->getClimbingPositionY(m_mob);

				sf::FloatRect checkBB = *m_mob->getBoundingBox();
				checkBB.top = climbingY;

				WorldCollisionQueryRecord rec;
				rec.boundingBox = *(m_mob->getBoundingBox());
				rec.boundingBox.top = climbingY;
				rec.ignoreDynamicTiles = m_mob->isIgnoreDynamicTiles();

				if (m_mob->getLevel()->collides(rec)) {
					g_logger->logWarning("UserMovingBehavior", "Cannot start climbing on this ladder, it would stuck the mob.");
					return;
				}

				startClimbing(tile, climbingY);
			}
		}
	}
}

void UserMovingBehavior::handleDefaultAcceleration() {
	if (m_isClimbing) {
		float newAccelerationX = m_mob->getAcceleration().x;
		m_mob->setAcceleration(sf::Vector2f(newAccelerationX, 0.f));
	}
	else {
		MovingBehavior::handleDefaultAcceleration();
	}
}

void UserMovingBehavior::handleMovementInput() {
	float newAccelerationX = m_mainChar->getAcceleration().x;
	if (!m_mainChar->isFeared()) {

		if (g_inputController->isKeyActive(Key::Left)) {
			if (!m_isClimbing) m_nextIsFacingRight = false;
			newAccelerationX -= m_isClimbing ? m_walkAcceleration * 0.2f : m_walkAcceleration;
		}
		if (g_inputController->isKeyActive(Key::Right)) {
			if (!m_isClimbing) m_nextIsFacingRight = true;
			newAccelerationX += m_isClimbing ? m_walkAcceleration * 0.2f : m_walkAcceleration;
		}
		if (!m_isClimbing && g_inputController->isKeyJustPressed(Key::Jump) && (m_isGrounded || m_jumpGraceTime > sf::Time::Zero)) {
			m_jumpGraceTime = sf::Time::Zero;
			m_mainChar->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp); // first jump vel will always be max y vel. 
		}
	}

	m_mainChar->setAcceleration(sf::Vector2f(newAccelerationX, m_isClimbing ? 0.f : (m_isFlippedGravity ? -m_gravity : m_gravity)));
}

void UserMovingBehavior::updateAnimation() {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_mainChar->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_isClimbing) {
		newState = m_isClimbingStep1 ? GameObjectState::Climbing_1 : GameObjectState::Climbing_2;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}
	else if (!m_isGrounded) {
		newState = GameObjectState::Jumping;
	}
	else if (std::abs(m_mainChar->getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_mainChar->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_mainChar->setState(newState, false);
		if (newState != GameObjectState::Dead)
			m_mainChar->setCurrentAnimation(m_mainChar->getAnimation(m_mainChar->getState()), !m_isFacingRight);
		else
			m_mainChar->loopCurrentAnimation(false);
	}
}
