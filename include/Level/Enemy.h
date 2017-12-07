#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "GUI/LootWindow.h"

#include "Structs/AttributeData.h"

#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/DamageType.h"
#include "Enums/EnemyState.h"

#include "Level/EnemyBuffBar.h"

class Level;
class LevelMainCharacter;
class Spell;
class EnemyMovingBehavior;
class EnemyAttackingBehavior;
class ScriptedBehavior;
class InteractComponent;

// the reward the player gets when this enemy is killed.
// includes loot and quest conditions.
struct EnemyReward final {
	// quest targets for this enemy
	std::vector<std::pair<std::string, std::string>> questTargets;
	// is this enemies death a condition for a quest?
	std::vector<std::pair<std::string, std::string>> questConditions;
	// lootable items 
	std::map<std::string, int> lootableItems;
	int lootableGold = 0;
};

// An enemy in a level
class Enemy : public virtual LevelMovableGameObject {
public:
	Enemy(const Level* level, Screen* screen);
	virtual ~Enemy();

	void load(int skinNr);

	void renderAfterForeground(sf::RenderTarget& target) override;
	void onRightClick() override;
	void onMouseOver() override;
	void update(const sf::Time& frameTime) override;

	void onHit(Spell* spell) override;
	void setFeared(const sf::Time& fearedTime) override;
	void setStunned(const sf::Time& stunnedTime) override;
	void addDamageOverTime(DamageOverTimeData& data) override;
	void setLoot(const std::map<std::string, int>& items, int gold);
	void addQuestTarget(const std::pair<std::string, std::string>& questtarget);
	void addQuestCondition(const std::pair<std::string, std::string>& questtarget);
	void setDead() override;
	void setDisposed() override;
	void setScriptedBehavior(const std::string& luaPath);
	void notifyKilled();
	void notifyLooted();
	bool isQuestRelevant();
	// the enemy tries to move to this target as long as it is not reset.
	void setMovingTarget(int x, int y);
	// resets the moving target
	void resetMovingTarget();
	// an enemy that is unique be marked as dead and not respawn with every level reset
	void setUnique(bool value);
	// the object ID in the level enemy object layer.
	void setObjectID(int id);
	// sets the enemy as ally. The enemy dies after the time to live has run out.
	virtual void setAlly(const sf::Time& ttl);
	void setEnemyName(const std::string& name);
	void setQuestRelevant(bool relevant);
	// switches the nearest lever
	void switchLever();

	GameObjectType getConfiguredType() const override;

	void setAlwaysUpdate();
	EnemyState getEnemyState() const;
	// a level, ranges from 0 to 4. An enemy can only be feared / stunned / resurrected, if the level of its spell is high enough.
	// default is 0. A enemy with level 4 can never be feared / stunned / controlled or affected in any other way!!
	virtual int getMentalStrength() const;
	// determines the distance from the top of the enemies bounding box to its
	// hp bar. Default is 20px.
	virtual float getConfiguredDistanceToHPBar() const;
	// returns whether the enemy is an ally of cendric
	bool isAlly() const override;
	// returns whether the enemy is a boss
	bool isBoss() const;
	// returns whether the enemy was already resurrected once
	bool isResurrected() const;
	// sets the waiting time to the configured waiting time
	void setWaiting();
	// sets the chasing time to the configured chasing time
	void setChasing();
	// sets the feared time to the configured feared time
	void setFleeing();
	// notifies this enemy that it has been resureccted
	void setResurrected();
	const LevelMovableGameObject* getCurrentTarget() const;

	void setTargeted(bool targeted);

	// returns false as a default. can be anything, for example if the enemy hp drops below some limit
	virtual bool getFleeCondition() const;

	// the default loot of a mob, can be looted when first killed (&looted)
	virtual void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {};
	// less loot, that can be still looted after a mob is looted 2-N times.
	virtual void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {};

	virtual EnemyID getEnemyID() const = 0;
	const std::string& getEnemyName() const;

	const ScriptedBehavior* getScriptedBehavior() const { return m_scriptedBehavior; }

protected:
	LevelMainCharacter* m_mainChar;
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	virtual void loadAttributes() = 0;
	// loads spells and adds them to the spell manager. default does nothing.
	virtual void loadSpells() = 0;
	// loads the animation
	virtual void loadAnimation(int skinNr) = 0;
	// loads/updates hp bar
	virtual void updateHpBar();
	// updates the hp bar after loading the behavior
	void loadBehavior() override;
	// loot, create the correct items + gold in the players inventory.
	void loot();

	int m_objectID = -1;
	bool m_isUnique = false;
	bool m_isBoss = false;
	bool m_isLooted = false;
	bool m_isResurrected = false;
	bool m_isHPBarVisible = true;
	bool m_isQuestRelevant = false; // marked as quest relevant

	// AI
	EnemyState m_enemyState = EnemyState::Idle;
	virtual void updateEnemyState(const sf::Time& frameTime);
	EnemyAttackingBehavior* m_enemyAttackingBehavior = nullptr;
	EnemyMovingBehavior* m_enemyMovingBehavior = nullptr;
	ScriptedBehavior* m_scriptedBehavior = nullptr;
	
	// time while the enemy does nothing because it tried to get to the main char and it failed
	sf::Time m_waitingTime = sf::Time::Zero;
	// time until next random desicion in idle state
	sf::Time m_decisionTime = sf::Time::Zero;
	// time the enemy will chase the main char anyway (even if it is out of aggro range)
	sf::Time m_chasingTime = sf::Time::Zero;

	virtual sf::Time getConfiguredRandomDecisionTime() const;
	// time feared after the fear condition is true (has nothing to do with spells)
	virtual sf::Time getConfiguredFearedTime() const;
	virtual sf::Time getConfiguredWaitingTime() const;
	virtual sf::Time getConfiguredChasingTime() const;

protected:
	InteractComponent* m_interactComponent;
	int m_skinNr;

	void clearReward();

private:
	sf::RectangleShape m_hpBar;
	static const float HP_BAR_HEIGHT;

	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;

	// the enemy can only be looted if the main char is in this range
	static const float PICKUP_RANGE;
	EnemyReward m_reward;
	std::string m_name = "";

	EnemyBuffBar* m_buffBar = nullptr;

	bool m_isAlly = false;
	// is this enemy currently targeted by the main character
	bool m_isTargetedEnemy = false;
	sf::Sprite m_targetSprite;
};