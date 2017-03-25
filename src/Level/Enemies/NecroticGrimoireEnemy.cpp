#include "Level/Enemies/NecroticGrimoireEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Level/DynamicTiles/DivineTorchTile.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::NecroticGrimoire, NecroticGrimoireEnemy)

const float NecroticGrimoireEnemy::TORCH_DISTANCE = 300.f;
const int NecroticGrimoireEnemy::ACTIVE_TORCH_COUNT = 3;

NecroticGrimoireEnemy::NecroticGrimoireEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen),
	BookEnemy(level, screen) {
	m_isInvincible = true;
}

void NecroticGrimoireEnemy::update(const sf::Time& frameTime) {
	BookEnemy::update(frameTime);
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
	m_attributes.setHealth(100);
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
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		} 
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
}

void NecroticGrimoireEnemy::loadAnimation(int skinNr) {
	skinNr = 1;
	BookEnemy::loadAnimation(skinNr);
}

void NecroticGrimoireEnemy::loadTorches() {
	for (auto* go : *m_screen->getObjects(GameObjectType::_DynamicTile)) {
		if (DivineTorchTile* tile = dynamic_cast<DivineTorchTile*>(go)) {
			m_torches.push_back(tile);
		}
	}

	m_hasLoadedTorches = true;
}