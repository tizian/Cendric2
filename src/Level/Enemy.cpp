#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"

using namespace std;

const float Enemy::HP_BAR_HEIGHT = 3.f;

Enemy::Enemy(Level* level, LevelMainCharacter* mainChar, EnemyID id) : LevelMovableGameObject(level) {
	m_id = id;
	m_mainChar = mainChar;
	m_immuneEnemies.push_back(id);
	m_attributes = ZERO_ATTRIBUTES;
	m_screen = mainChar->getScreen();
	m_spellManager = new SpellManager(this);
	m_questTarget.first = "";

	// load hp bar
	m_hpBar.setFillColor(sf::Color::Red);
	updateHpBar();

	m_buffBar = new EnemyBuffBar(this);
}

Enemy::~Enemy() {
	delete m_lootWindow;
	delete m_buffBar;
}

bool Enemy::getFleeCondition() const {
	return false;
}

void Enemy::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	bool collidesX = false;
	if (isMovingX && m_level->collides(nextBoundingBoxX, m_ignoreDynamicTiles)) {
		collidesX = true;
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	// check for collision on y axis
	bool collidesY = m_level->collides(nextBoundingBoxY, m_ignoreDynamicTiles);

	if (!isMovingDown && collidesY) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
		// set mob up in case of anti gravity!
		if (getIsUpsideDown()) {
			setPositionY(m_level->getCeiling(nextBoundingBoxY));
			m_isGrounded = true;
			if (!m_isDead && m_level->collidesLevelCeiling(nextBoundingBoxY)) {
				// colliding with level ceiling is deadly when the mob is upside down.
				setDead();
			}
		}
	}
	else if (isMovingDown && collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		// set mob down. in case of normal gravity.
		if (!getIsUpsideDown()) {
			setPositionY(m_level->getGround(nextBoundingBoxY));
			m_isGrounded = true;
			if (!m_isDead && m_level->collidesLevelBottom(nextBoundingBoxY)) {
				// colliding with level bottom is deadly.
				setDead();
			}
		}
	}

	m_jumps = false;
	if (isMovingX && collidesX) {
		// would a jump work? 
		m_jumps = !m_level->collidesAfterJump(*getBoundingBox(), m_jumpHeight, m_isFacingRight, m_ignoreDynamicTiles);
	}

	// checks if the enemy falls would fall deeper than it can jump. 
	if (isMovingX && m_level->fallsDeep(*getBoundingBox(), m_jumpHeight, m_isFacingRight, getDistanceToAbyss(), m_ignoreDynamicTiles)) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
		collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
	}

	if (std::abs(getVelocity().y) > 0.f)
		m_isGrounded = false;

	// if the enemy collidesX but can't jump and is chasing, it waits for a certain time.
	// the same if it can't reach the main char because of the y difference.
	if (m_enemyState == EnemyState::Chasing && ((collidesX && !m_jumps) || abs(m_mainChar->getPosition().y - getPosition().y) > 2 * m_jumpHeight)) {
		m_waitingTime = getConfiguredWaitingTime();
	}
}

void Enemy::onHit(Spell* spell) {
	if (m_isDead) {
		return;
	}
	// check for owner
	if (spell->getOwner() == this) {
		return;
	}
	// check for immune damage types, if yes, the spell will disappear, absorbed by the immuneness of this enemy
	if (std::find(m_immuneDamageTypes.begin(), m_immuneDamageTypes.end(), spell->getDamageType()) != m_immuneDamageTypes.end()) {
		spell->setDisposed();
		return;
	}
	LevelMovableGameObject::onHit(spell);
	m_chasingTime = getConfiguredChasingTime();
	m_recoveringTime = getConfiguredRecoveringTime();
}

void Enemy::renderAfterForeground(sf::RenderTarget &renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	m_buffBar->render(renderTarget);
	renderTarget.draw(m_hpBar);
	if (m_showLootWindow && m_lootWindow != nullptr) {
		m_lootWindow->render(renderTarget);
		m_showLootWindow = false;
	}
}

void Enemy::update(const sf::Time& frameTime) {
	updateEnemyState(frameTime);
	updateAggro();
	LevelMovableGameObject::update(frameTime);
	updateHpBar();
	if (m_showLootWindow && m_lootWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left + getBoundingBox()->width, getBoundingBox()->top - m_lootWindow->getSize().y + 10.f);
		m_lootWindow->setPosition(pos);
	}
	m_showLootWindow = m_showLootWindow || g_inputController->isKeyActive(Key::ToggleTooltips);
	m_buffBar->update(frameTime);
}

void Enemy::updateHpBar() {
	m_hpBar.setPosition(getBoundingBox()->left, getBoundingBox()->top - getConfiguredDistanceToHPBar());
	m_hpBar.setSize(sf::Vector2f(getBoundingBox()->width * (static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints), HP_BAR_HEIGHT));
}

float Enemy::distToMainChar() const {
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Time Enemy::getConfiguredRecoveringTime() const {
	return sf::milliseconds(200);
}

sf::Time Enemy::getConfiguredWaitingTime() const {
	return sf::seconds(2);
}

sf::Time Enemy::getConfiguredRandomDecisionTime() const {
	int r = rand() % 1500 + 200;
	return sf::milliseconds(r);
}

sf::Time Enemy::getConfiguredFearedTime() const {
	return sf::seconds(6);
}

sf::Time Enemy::getConfiguredChasingTime() const {
	return sf::seconds(1);
}

float Enemy::getDistanceToAbyss() const {
	return 10.f;
}

GameObjectType Enemy::getConfiguredType() const {
	return GameObjectType::_Enemy;
}

void Enemy::updateEnemyState(const sf::Time& frameTime) {
	// handle dead
	if (m_enemyState == EnemyState::Dead) return;

	// update times
	GameObject::updateTime(m_stunnedTime, frameTime);
	GameObject::updateTime(m_waitingTime, frameTime);
	GameObject::updateTime(m_recoveringTime, frameTime);
	GameObject::updateTime(m_fearedTime, frameTime);
	GameObject::updateTime(m_chasingTime, frameTime);
	GameObject::updateTime(m_decisionTime, frameTime);

	// handle stunned
	if (m_stunnedTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Stunned;
		return;
	}

	// handle fear
	if (m_fearedTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Fleeing;
		return;
	}

	// handle recovering
	if (m_recoveringTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Recovering;
		return;
	}

	// handle chasing
	if (m_chasingTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Chasing;
		return;
	}

	// handle waiting
	if (m_waitingTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Waiting;
	}
	else {
		m_enemyState = EnemyState::Idle;
	}

	if ((m_enemyState == EnemyState::Waiting || m_enemyState == EnemyState::Idle) &&
		m_decisionTime == sf::Time::Zero) {
		// decide again
		m_decisionTime = getConfiguredRandomDecisionTime();
		m_randomDecision = rand() % 3 - 1;
	}
}

void Enemy::handleMovementInput() {
	// movement AI
	float newAccelerationX = 0;

	if (m_enemyState == EnemyState::Chasing || m_enemyState == EnemyState::Recovering) {

		if (m_mainChar->getCenter().x < getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > getApproachingDistance()) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}

		if (m_mainChar->getCenter().x > getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > getApproachingDistance()) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_isGrounded) {
			setVelocityY(m_isFlippedGravity ? getConfiguredMaxVelocityYUp() : -getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (m_enemyState == EnemyState::Fleeing) {

		if (m_mainChar->getCenter().x < getCenter().x) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}

		if (m_mainChar->getCenter().x > getCenter().x) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_isGrounded) {
			setVelocityY(-getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (m_enemyState == EnemyState::Idle || m_enemyState == EnemyState::Waiting) {

		if (m_randomDecision != 0) {
			m_nextIsFacingRight = (m_randomDecision == 1);
			newAccelerationX += (m_randomDecision * getConfiguredWalkAcceleration());
		}
	}

	setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -getGravityAcceleration() : getGravityAcceleration())));
}

void Enemy::updateAggro() {
	float invisibilityScaler;
	int invLevel = m_mainChar->getInvisibilityLevel();
	int mentalStr = getMentalStrength();
	if (invLevel == 0) {
		invisibilityScaler = 1.f;
	}
	else if (invLevel > mentalStr) {
		invisibilityScaler = 0.f;
	}
	else {
		invisibilityScaler = 1.f / (2 * (6 - mentalStr)) + 1.f / (invLevel + 1);
	}
	
	bool isInAggroRange = distToMainChar() < (getAggroRange() * invisibilityScaler);

	if (m_enemyState == EnemyState::Chasing && getFleeCondition()) {
		m_fearedTime = getConfiguredFearedTime();
		return;
	}

	if ((m_enemyState == EnemyState::Idle || m_enemyState == EnemyState::Chasing) && isInAggroRange) {
		m_chasingTime = getConfiguredChasingTime();
		return;
	}
}

EnemyID Enemy::getEnemyID() const {
	return m_id;
}

float Enemy::getConfiguredDistanceToHPBar() const {
	return 20.f;
}

int Enemy::getMentalStrength() const {
	return 0;
}

void Enemy::setLoot(const std::map<string, int>& items, int gold) {
	m_lootableItems = items;
	m_lootableGold = gold;
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void Enemy::setQuestTarget(const std::pair<std::string, std::string>& questtarget) {
	m_questTarget = questtarget;
}

void Enemy::setObjectID(int id) {
	m_objectID = id;
}

void Enemy::setFeared(const sf::Time &fearedTime) {
	m_fearedTime = fearedTime;
	m_buffBar->addFeared(fearedTime);
}

void Enemy::setStunned(const sf::Time &stunnedTime) {
	m_stunnedTime = stunnedTime;
	m_buffBar->addStunned(stunnedTime);
}

void Enemy::addDamageOverTime(const DamageOverTimeData& data) {
	if (m_isDead) return;
	m_buffBar->addDotBuff(data.duration, data.damageType);
	LevelMovableGameObject::addDamageOverTime(data);
}

void Enemy::onMouseOver() {
	if (m_isDead) {
		setSpriteColor(sf::Color::Red, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void Enemy::onRightClick() {
	if (m_isDead) {
		// check if the enemy body is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();

		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE) {
			// loot, create the correct items + gold in the players inventory.
			m_mainChar->lootItems(m_lootableItems);
			m_mainChar->addGold(m_lootableGold);
			m_screen->getCharacterCore()->setEnemyLooted(m_mainChar->getLevel()->getID(), m_objectID);
			setDisposed();
		}
		else {
			m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
		}
		g_inputController->lockAction();
	}
}

void Enemy::setDead() {
	LevelMovableGameObject::setDead();
	m_buffBar->clear();
	if (m_screen->getCharacterCore()->isEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID)) return;

	m_screen->getCharacterCore()->setEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID);

	if (!m_questTarget.first.empty()) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyQuestTargetKilled(m_questTarget.first, m_questTarget.second);
	}
}