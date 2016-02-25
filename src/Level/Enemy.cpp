#include "Level/Enemy.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"

using namespace std;

const float Enemy::HP_BAR_HEIGHT = 3.f;
const float Enemy::PICKUP_RANGE = 100.f;

Enemy::Enemy(Level* level, Screen* screen) : LevelMovableGameObject(level) {
	m_mainChar = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	m_attributes = ZERO_ATTRIBUTES;
	m_screen = screen;
	m_spellManager = new SpellManager(this);
	m_questTarget.first = "";

	m_buffBar = new EnemyBuffBar(this);
}

Enemy::~Enemy() {
	delete m_lootWindow;
	delete m_buffBar;
	delete m_scriptedBehavior;
}

void Enemy::load(EnemyID id) {
	m_id = id;
	loadAnimation();
	loadAttributes();
	loadSpells();
	loadBehavior();
	m_spellManager->setSpellsAllied(m_isAlly);
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
	LevelMovableGameObject::update(frameTime);
	if (m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
	}
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

void Enemy::loadBehavior() {
	LevelMovableGameObject::loadBehavior();
	// cast the two behavior components so that we only have to cast them once.
	m_enemyAttackingBehavior = dynamic_cast<EnemyAttackingBehavior*>(m_attackingBehavior);
	m_enemyMovingBehavior = dynamic_cast<EnemyMovingBehavior*>(m_movingBehavior);
	if (m_enemyAttackingBehavior == nullptr || m_enemyMovingBehavior == nullptr) {
		g_logger->logError("Enemy", "Enemies can only have valid enemy behaviors!");
	}
	// hp bar fill color is dependent on attacking behavior
	m_hpBar.setFillColor(m_enemyAttackingBehavior->getConfiguredHealthColor());
	updateHpBar();
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

GameObjectType Enemy::getConfiguredType() const {
	return GameObjectType::_Enemy;
}

void Enemy::updateEnemyState(const sf::Time& frameTime) {
	// handle dead
	if (m_enemyState == EnemyState::Dead) return;

	// update times
	GameObject::updateTime(m_recoveringTime, frameTime);
	GameObject::updateTime(m_waitingTime, frameTime);
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
		m_enemyMovingBehavior->makeRandomDecision();
	}
}

bool Enemy::isAlly() const {
	return m_isAlly;
}

const LevelMovableGameObject* Enemy::getCurrentTarget() const {
	return m_enemyAttackingBehavior->getCurrentTarget();
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

EnemyState Enemy::getEnemyState() const {
	return m_enemyState;
}

void Enemy::setWaiting() {
	m_waitingTime = getConfiguredWaitingTime();
}

void Enemy::setChasing() {
	m_chasingTime = getConfiguredChasingTime();
}

void Enemy::setFleeing() {
	m_fearedTime = getConfiguredFearedTime();
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

void Enemy::setAlly(const sf::Time& ttl) {
	m_isAlly = true;
	delete m_movingBehavior;
	m_movingBehavior = createMovingBehavior(true);
	m_enemyMovingBehavior = dynamic_cast<EnemyMovingBehavior*>(m_movingBehavior);

	delete m_attackingBehavior;
	m_attackingBehavior = createAttackingBehavior(true);
	AllyBehavior* allyBehavior = dynamic_cast<AllyBehavior*>(m_attackingBehavior);
	if (allyBehavior == nullptr) {
		g_logger->logError("Enemy::setAlly", "Enemy can't be risen as an ally, no AllyBehavior created.");
		return;
	}
	allyBehavior->setTimeToLive(ttl);
	m_enemyAttackingBehavior = allyBehavior;
	m_hpBar.setFillColor(m_enemyAttackingBehavior->getConfiguredHealthColor());
	m_isAlwaysUpdate = true;

	m_spellManager->setSpellsAllied(true);

	setDebugBoundingBox(sf::Color::Green);
}

void Enemy::setPersistent(bool value) {
	m_isPersistent = value;
}

void Enemy::setFeared(const sf::Time& fearedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setFeared(fearedTime);
	m_buffBar->addFeared(fearedTime);
}

void Enemy::setStunned(const sf::Time& stunnedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setStunned(stunnedTime);
	m_buffBar->addStunned(stunnedTime);
}

void Enemy::addDamageOverTime(DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	m_buffBar->addDotBuff(data.duration, data.damageType);
	LevelMovableGameObject::addDamageOverTime(data);
}

void Enemy::onMouseOver() {
	if (m_isDead && !isAlly()) {
		setSpriteColor(sf::Color::Red, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void Enemy::onRightClick() {
	if (m_isDead && !isAlly()) {
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

void Enemy::setScriptedBehavior(const std::string& luaPath) {
	delete m_scriptedBehavior;
	m_scriptedBehavior = new ScriptedBehavior(luaPath, m_screen->getCharacterCore(), this);
	if (m_scriptedBehavior->isError()) {
		delete m_scriptedBehavior;
		m_scriptedBehavior = nullptr;
	}
}

void Enemy::setDead() {
	if (m_isImmortal) return;
	LevelMovableGameObject::setDead();
	m_buffBar->clear();

	if (isAlly()) {
		setDisposed();
		return;
	}

	if (!m_isPersistent) {
		notifyKilled();
	}
}

void Enemy::notifyKilled() {
	if (m_screen->getCharacterCore()->isEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID)) return;
	m_screen->getCharacterCore()->setEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID);
	if (!m_questTarget.first.empty()) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyQuestTargetKilled(m_questTarget.first, m_questTarget.second);
	}
}