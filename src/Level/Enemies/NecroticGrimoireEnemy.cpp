#include "Level/Enemies/NecroticGrimoireEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Screens/LevelScreen.h"
#include "Level/DynamicTiles/DivineTorchTile.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::NecroticGrimoire, NecroticGrimoireEnemy)

const float NecroticGrimoireEnemy::TORCH_DISTANCE = 500.f;
const int NecroticGrimoireEnemy::ACTIVE_TORCH_COUNT = 3;
const sf::Time NecroticGrimoireEnemy::SUMMON_TIME = sf::seconds(5);

NecroticGrimoireEnemy::NecroticGrimoireEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen),
	BookEnemy(level, screen) {
	m_isInvincible = true;
	m_isAlwaysUpdate = true;
}

void NecroticGrimoireEnemy::update(const sf::Time& frameTime) {
	BookEnemy::update(frameTime);
	updateTime(m_summonTime, frameTime);
	if (!m_hasLoadedTorches) {
		loadTorches();
	}
	
	// check near divine tiles
	int torchCount = 0;
	for (auto torch : m_torches) {
		if (torch->getGameObjectState() != GameObjectState::Burning) continue;
		if (dist(torch->getCenter(), getCenter()) < TORCH_DISTANCE) {
			torch->setTarget(this);
			torchCount++;
		}
		else {
			torch->setTarget(nullptr);
		}
	}

	m_isInvincible = torchCount < ACTIVE_TORCH_COUNT;
}

void NecroticGrimoireEnemy::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistancePhysical = 10;
	m_attributes.resistanceIce = 10;
	m_attributes.resistanceShadow = 10000;
	m_attributes.resistanceLight = -20;
	m_attributes.resistanceFire = -20;
	m_attributes.calculateAttributes();
}

void NecroticGrimoireEnemy::loadSpells() {
	SpellData pageSpell = SpellData::getSpellData(SpellID::RotatingProjectile);
	pageSpell.isDynamicTileEffect = false;
	pageSpell.damage = 5;
	pageSpell.fightingTime = sf::seconds(0.f);
	pageSpell.castingTime = sf::seconds(0.f);
	pageSpell.castingAnimation = GameObjectState::VOID;
	pageSpell.fightAnimation = GameObjectState::VOID;
	pageSpell.speed = 400.f;
	pageSpell.cooldown = sf::seconds(1.f);

	m_spellManager->addSpell(pageSpell);
	m_spellManager->setCurrentSpell(0);
}

AttackingBehavior* NecroticGrimoireEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&NecroticGrimoireEnemy::handleAttackInput, this));
	return behavior;
}

void NecroticGrimoireEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 600.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
		m_chasingTime = sf::Time::Zero;
		m_waitingTime = sf::seconds(static_cast<float>(rand() % 4 + 1));
		if (m_summonTime == sf::Time::Zero) {
			summonEnemy();
			m_summonTime = SUMMON_TIME;
		}
	}
}

void NecroticGrimoireEnemy::summonEnemy() {
	Enemy* enemy = nullptr;
	enemy = ObjectFactory::Instance()->createEnemy(EnemyID::Skeleton_Default, dynamic_cast<LevelScreen*>(m_screen)->getWorld(), m_screen);
	if (enemy == nullptr) {
		g_logger->logError("NecroticGrimoireEnemy", "Enemy could not be spawned, unknown id.");
		return;
	}
	enemy->load(0);

	enemy->setLoot(std::map<std::string, int>(), 0);
	enemy->setPosition(getPosition());
	enemy->setObjectID(-1);
	enemy->setUnique(false);
	enemy->setDebugBoundingBox(sf::Color::Magenta);
	m_screen->addObject(enemy);
}

void NecroticGrimoireEnemy::loadAnimation(int skinNr) {
	skinNr = 1;
	BookEnemy::loadAnimation(skinNr);
}

void NecroticGrimoireEnemy::setDead() {
	if (m_isDead) return;
	BookEnemy::setDead();
	for (auto torch : m_torches) {
		torch->setTarget(nullptr);
	}
	m_torches.clear();
}

void NecroticGrimoireEnemy::loadTorches() {
	for (auto* go : *m_screen->getObjects(GameObjectType::_DynamicTile)) {
		if (DivineTorchTile* tile = dynamic_cast<DivineTorchTile*>(go)) {
			m_torches.push_back(tile);
		}
	}

	m_hasLoadedTorches = true;
}