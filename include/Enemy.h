#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "GUI/LootWindow.h"

#include "Structs/AttributeBean.h"

#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/ItemID.h"

class Level;
class LevelMainCharacter;
class Spell;

enum class EnemyState {
	Idle,
	Chasing,
	// the enemy has taken a hit and needs to recover 
	Recovering,
	Fleeing
};

// An enemy in a level
class Enemy : public LevelMovableGameObject
{
public:
	Enemy(Level* level, LevelMainCharacter* mainChar, EnemyID id);
	~Enemy();

	virtual void load() = 0;
	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void render(sf::RenderTarget& target) override;
	void onRightClick() override;
	void onMouseOver() override;
	void update(const sf::Time& frameTime) override;
	void addDamage(int damage) override;
	void setDead() override;

	virtual void onHit(Spell* spell);
	void setLoot(const std::map<ItemID, int>& items, int gold);

	GameObjectType getConfiguredType() const override;
	EnemyID getEnemyID() const;
	// determines the distance from the top of the enemies bounding box to its
	// hp bar. Default is 20px.
	virtual float getConfiguredDistanceToHPBar() const;

protected:
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	virtual void loadAttributes() = 0;
	// loads spells and adds them to the spell manager. default does nothing.
	virtual void loadSpells() = 0;
	// loads/updates hp bar
	virtual void updateHpBar();
	
	EnemyID m_id;
	// spells from these enemies won't hurt. default is its own type.
	std::vector<EnemyID> m_immuneEnemies;
	// spells of these types won't hurt. default is empty.
	std::vector<SpellID> m_immuneSpells;
	// the attribute bean. if the enemies hp falls to 0, its state will be dead.
	AttributeBean m_attributes;
	
	// AI
	EnemyState m_enemyState;
	virtual void updateEnemyState(const sf::Time& frameTime);
	// if the enemy is not in this range, its state is idle and it will do some random movement.
	// inside, it will probably chase & attack the main char.
	virtual float getConfiguredAggroRange() const = 0;
	// returns false as a default. can be anything, for example if the enemy hp drops below some limit
	virtual bool getConfiguredFleeCondition() const;
	// how near does an enemy go to the abyss until it stops? default is 10.f. Can be 0 for unflinching enemies
	virtual float getConfiguredDistanceToAbyss() const;
	virtual sf::Time getConfiguredFightAnimationTime() const = 0;
	// the distance from the center of the enemy to the center of the main char at which the enemy approaches the main char.
	virtual float getConfiguredApproachingDistance() const = 0;
	// the target to be destroyed!
	LevelMainCharacter* m_mainChar;
	float distToMainChar() const;
	// if this bool is set to true, the enemy jumps in the next frame. used by the AI
	bool m_jumps = false;
	// a descision the enemy has taken an that lasts until it decides anew. -1: walk left, 0: stay, 1: walk right
	int m_randomDescision = 0;
	// time until the enemy can do anything after it has taken a hit or a special spell (stun)
	sf::Time m_recoveringTime = sf::Time::Zero;
	// time until next random desicion in idle state
	sf::Time m_descisionTime = sf::Time::Zero;

	virtual void handleMovementInput() override;
	virtual sf::Time getConfiguredRecoveringTime() const;
	virtual sf::Time getRandomDescisionTime() const;
	float m_jumpHeight = 0;

private:
	sf::RectangleShape m_hpBar;
	const float HP_BAR_HEIGHT = 3.f;

	// lootable items 
	std::map<ItemID, int> m_lootableItems;
	int m_lootableGold;
	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;
	
	// the enemy can only be looted if the main char is in this range
	const float PICKUP_RANGE = 100.f;
};