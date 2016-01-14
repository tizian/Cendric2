#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"

using namespace std;

const float Enemy::HP_BAR_HEIGHT = 3.f;
const float Enemy::PICKUP_RANGE = 100.f;

Enemy::Enemy(Level* level, Screen* screen, bool isControlled) : LevelMovableGameObject(level) {
	m_mainChar = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_LevelMainCharacter)->at(0));
	m_enemies = screen->getObjects(GameObjectType::_Enemy);
	m_isControlled = isControlled;
	m_attributes = ZERO_ATTRIBUTES;
	m_screen = screen;
	m_spellManager = new SpellManager(this);
	m_questTarget.first = "";

	// load hp bar
	m_hpBar.setFillColor(m_isControlled ? sf::Color::Green : sf::Color::Red);
	updateHpBar();

	m_buffBar = new EnemyBuffBar(this);

	if (!m_isControlled) {
		m_currentTarget = m_mainChar;
	}
}

Enemy::~Enemy() {
	delete m_lootWindow;
	delete m_buffBar;
}

bool Enemy::getFleeCondition() const {
	return false;
}

void Enemy::onHit(Spell* spell) {
	if (m_isDead) {
		if (spell->getSpellID() == SpellID::RaiseTheDead)
			spell->execOnHit(this);
		return;
	}
	// check for owner
	if (const Enemy* enemy = dynamic_cast<const Enemy*>(spell->getOwner())) {
		if (m_isControlled && enemy->isControlled() || !m_isControlled && !enemy->isControlled()) {
			return;
		}
	}
	if (m_isControlled && spell->getOwner()->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
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

	if (m_isControlled) {
		GameObject::updateTime(m_timeToLive, frameTime);
		if (m_timeToLive == sf::Time::Zero) {
			setDead();
		}
	}
}

void Enemy::updateHpBar() {
	m_hpBar.setPosition(getBoundingBox()->left, getBoundingBox()->top - getConfiguredDistanceToHPBar());
	m_hpBar.setSize(sf::Vector2f(getBoundingBox()->width * (static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints), HP_BAR_HEIGHT));
}

float Enemy::distToTarget() const {
	if (m_currentTarget == nullptr) return 10000.f;
	sf::Vector2f dist = m_currentTarget->getCenter() - getCenter();
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Time Enemy::getConfiguredRecoveringTime() const {
	return sf::milliseconds(200);
}

sf::Time Enemy::getConfiguredWaitingTime() const {
	return sf::seconds(1.f);
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

bool Enemy::isControlled() const {
	return m_isControlled;
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

	// the state must have been chasing a frame before. Wait now.
	if (m_enemyState == EnemyState::Chasing) {
		m_waitingTime = getConfiguredWaitingTime();
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
		makeRandomDecision();
	}
}

void Enemy::updateAggro() {
	if (m_enemyState == EnemyState::Chasing && getFleeCondition()) {
		m_fearedTime = getConfiguredFearedTime();
		return;
	}
	if (m_currentTarget == nullptr || m_currentTarget->isDead() || m_currentTarget->isDisposed()) {
		m_currentTarget = nullptr;
	}
	if (m_enemyState != EnemyState::Idle) return;

	bool isInAggroRange = false;
	if (!m_isControlled) {
		// handle main character aggro
		float invisibilityScaler = 1.f;
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
		isInAggroRange = distToTarget() < (getAggroRange() * invisibilityScaler);
		if (isInAggroRange) {
			m_chasingTime = getConfiguredChasingTime();
			return;
		}
	}
	else {
		// search for new target
		Enemy* nearest = nullptr;
		float nearestDistance = 10000.f;
		for (auto& go : *m_enemies) {
			if (!go->isViewable()) continue;
			Enemy* enemy = dynamic_cast<Enemy*>(go);
			if (enemy->isDead() || enemy->isControlled()) continue;
			sf::Vector2f dist = go->getCenter() - getCenter();
			float distance = sqrt(dist.x * dist.x + dist.y * dist.y);
			if (distance < nearestDistance) {
				nearestDistance = distance;
				nearest = enemy;
			}
		}
		if (nearest == nullptr) {
			m_currentTarget = nullptr;
			m_waitingTime = getConfiguredWaitingTime();
			return;
		}
		m_currentTarget = nearest;
		m_chasingTime = getConfiguredChasingTime();
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

void Enemy::setTimeToLive(const sf::Time& ttl) {
	if (!m_isControlled) return;
	m_timeToLive = ttl;
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
	if (m_isDead && !m_isControlled) {
		setSpriteColor(sf::Color::Red, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void Enemy::onRightClick() {
	if (m_isDead && !m_isControlled) {
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

	if (m_isControlled) {
		setDisposed();
		return;
	}

	if (m_screen->getCharacterCore()->isEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID)) return;
	m_screen->getCharacterCore()->setEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID);
	if (!m_questTarget.first.empty()) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyQuestTargetKilled(m_questTarget.first, m_questTarget.second);
	}
}