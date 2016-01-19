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

// An enemy in a level
class Enemy : virtual public LevelMovableGameObject {
public:
	Enemy(Level* level, Screen* screen, bool isControlled = false);
	virtual ~Enemy();

	virtual void load() = 0;

	void renderAfterForeground(sf::RenderTarget& target) override;
	void onRightClick() override;
	void onMouseOver() override;
	void update(const sf::Time& frameTime) override;

	void onHit(Spell* spell) override;
	// the enemy flees for the given time
	void setFeared(const sf::Time& fearedTime);
	// the enemy is stunned for the given time
	void setStunned(const sf::Time& stunnedTime);
	void addDamageOverTime(const DamageOverTimeData& data) override;
	void setLoot(const std::map<std::string, int>& items, int gold);
	void setQuestTarget(const std::pair<std::string, std::string>& questtarget);
	void setDead() override;
	// an enemy that is persistent will not be marked as dead and respawn with every level reset
	void setPersistent(bool value);
	// the object ID in the level enemy object layer.
	void setObjectID(int id);
	// sets the time to live for a controlled enemy
	void setTimeToLive(const sf::Time& ttl);

	bool isControlled() const;
	GameObjectType getConfiguredType() const override;
	EnemyID getEnemyID() const;
	// a level, ranges from 0 to 4. An enemy can only be feared / stunned / resurrected, if the level of its spell is high enough.
	// default is 0. A enemy with level 4 can never be feared / stunned / controlled or affected in any other way!!
	virtual int getMentalStrength() const;
	// determines the distance from the top of the enemies bounding box to its
	// hp bar. Default is 20px.
	virtual float getConfiguredDistanceToHPBar() const;
	virtual void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const = 0;

protected:
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	virtual void loadAttributes() = 0;
	// loads spells and adds them to the spell manager. default does nothing.
	virtual void loadSpells() = 0;
	// loads/updates hp bar
	virtual void updateHpBar();

	EnemyID m_id = EnemyID::VOID;
	int m_objectID = -1;
	bool m_isPersistent = false;
	// spells of these damage types won't hurt. default is empty.
	std::vector<DamageType> m_immuneDamageTypes;
	// when this bool is set to true, the enemy will help the player instead of hurting it
	bool m_isControlled;
	sf::Time m_timeToLive = sf::Time::Zero;

	// AI
	EnemyState m_enemyState = EnemyState::Idle;
	virtual void updateEnemyState(const sf::Time& frameTime);
	virtual void updateAggro();
	// if the enemy is not in this range, its state is idle and it will do some random movement.
	// inside, it will probably chase & attack the main char.
	virtual float getAggroRange() const = 0;
	// returns false as a default. can be anything, for example if the enemy hp drops below some limit
	virtual bool getFleeCondition() const;
	// the distance from the center of the enemy to the center of the main char at which the enemy approaches the main char.
	virtual float getApproachingDistance() const = 0;
	// the target to be destroyed!
	LevelMovableGameObject* m_currentTarget = nullptr;
	LevelMainCharacter* m_mainChar;
	std::vector<GameObject*>* m_enemies;
	float distToTarget() const;
	virtual void makeRandomDecision() = 0;
	// time until the enemy can attack after it has taken a hit
	sf::Time m_recoveringTime = sf::Time::Zero;
	// time stunned
	sf::Time m_stunnedTime = sf::Time::Zero;
	// time feared
	sf::Time m_fearedTime = sf::Time::Zero;
	// time while the enemy does nothing because it tried to get to the main char and it failed
	sf::Time m_waitingTime = sf::Time::Zero;
	// time until next random desicion in idle state
	sf::Time m_decisionTime = sf::Time::Zero;
	// time the enemy will chase the main char anyway (even if it is out of aggro range)
	sf::Time m_chasingTime = sf::Time::Zero;

	virtual sf::Time getConfiguredRecoveringTime() const;
	virtual sf::Time getConfiguredRandomDecisionTime() const;
	// time feared after the fear condition is true (has nothing to do with spells)
	virtual sf::Time getConfiguredFearedTime() const;
	virtual sf::Time getConfiguredWaitingTime() const;
	virtual sf::Time getConfiguredChasingTime() const;

private:
	sf::RectangleShape m_hpBar;
	static const float HP_BAR_HEIGHT;

	// lootable items 
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold;
	LootWindow *m_lootWindow = nullptr;
	bool m_showLootWindow = false;

	// is this enemy a quest target?
	std::pair<std::string, std::string> m_questTarget;

	// the enemy can only be looted if the main char is in this range
	static const float PICKUP_RANGE;

	EnemyBuffBar* m_buffBar = nullptr;
};