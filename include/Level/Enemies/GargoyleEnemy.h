#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

// A flying Gargoyle that is mainly used for the necro spell "summon gargoyle"
// but can also appear as a normal enemy.
class GargoyleEnemy : public Enemy {
public:
	GargoyleEnemy(const Level* level, Screen* screen);
	~GargoyleEnemy() {}

	void update(const sf::Time& frameTime) override;
	void loadAnimation() override;
	void loadAnimation(int skinNr);
	void setDead() override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	sf::Vector2f getConfiguredSpellOffset() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	// called from the spell that summons this gargoyle.
	// the parameters are set by the spell.
	void setSummoned(int strength, int damage, int count, const sf::Time& ttl);
	bool isSummoned() const;

protected:
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;

	int m_spellStrength = 1;
	int m_spellAdditionalDamage = 0;
	int m_spellCount = 1;
	bool m_isSummoned = false;
	int m_maxSpell = 0;

	sf::Time m_attackWaitTime = sf::Time::Zero;
};