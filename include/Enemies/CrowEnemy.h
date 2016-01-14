#pragma once

#include "global.h"
#include "Level/FlyingEnemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A flying crow
class CrowEnemy : public FlyingEnemy {
public:
	CrowEnemy(Level* level, Screen* screen, bool isControlled = false);
	~CrowEnemy() {}

	void load() override;

	float getMaxVelocityYUp() const override;
	float getMaxVelocityYDown() const override;
	float getMaxVelocityX() const override;
	float getApproachingDistance() const override;
	float getAggroRange() const override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;
	sf::Time getConfiguredFightAnimationTime() const override;
	sf::Vector2f getConfiguredSpellOffset() const override;

	static void insertDefaultLoot(std::map<std::string, int>& loot, int& gold);
	Enemy* createNewControlledInstance(const sf::Time& ttl, const AttributeData& additionalAttributes) const override;

protected:
	void handleAttackInput() override;
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
};