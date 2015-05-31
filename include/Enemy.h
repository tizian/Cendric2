#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"

#include "Structs/AttributeBean.h"

#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/ItemID.h"

class Level;
class LevelMainCharacter;
class Spell;

// An enemy in a level
class Enemy : public LevelMovableGameObject
{
public:
	Enemy(Level* level, LevelMainCharacter* mainChar, EnemyID id);

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void render(sf::RenderTarget& target) override;
	void onRightClick() override;
	void onMouseOver() override;
	void update(const sf::Time& frameTime) override;

	virtual void onHit(Spell* spell);
	void setLoot(const std::map<ItemID, int>& items, int gold);

	GameObjectType getConfiguredType() const override;
	EnemyID getEnemyID() const;
	virtual sf::Color getConfiguredDebugColor() const override;
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
	// the target to be destroyed!
	LevelMainCharacter* m_mainChar;
	float distToMainChar() const;
	// if this bool is set to true, the enemy jumps in the next frame. used by the AI
	bool m_jumps = false;
	EnemyID m_id;
	// spells from these enemies won't hurt. default is its own type.
	std::vector<EnemyID> m_immuneEnemies;
	// spells of these types won't hurt. default is empty.
	std::vector<SpellID> m_immuneSpells;
	// the attribute bean. if the enemies hp falls to 0, its state will be dead.
	AttributeBean m_attributes;

private:
	sf::RectangleShape m_hpBar;
	const float HP_BAR_HEIGHT = 3.f;

	// lootable items 
	std::map<ItemID, int> m_lootableItems;
	int m_lootableGold;
	// the enemy can only be looted if the main char is in this range
	const float PICKUP_RANGE = 100.f;
};