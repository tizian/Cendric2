#pragma once

#include "global.h"
#include "Level/WalkingEnemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A spooky Nekomata in a level
class NekomataEnemy : public WalkingEnemy {
public:
	NekomataEnemy(Level* level, Screen* screen, bool isControlled = false);
	virtual ~NekomataEnemy() {}

	void load() override;

	float getApproachingDistance() const override;
	float getAggroRange() const override;
	sf::Time getConfiguredFightAnimationTime() const override;
	float getMaxVelocityYUp() const override;
	float getMaxVelocityYDown() const override;
	float getMaxVelocityX() const override;
	sf::Vector2f getConfiguredSpellOffset() const override;
	float getDistanceToAbyss() const override;
	void updateAnimation(const sf::Time& frameTime) override;

	static void insertDefaultLoot(std::map<std::string, int>& loot, int& gold);
	Enemy* createNewControlledInstance(const sf::Time& ttl, const AttributeData& additionalAttributes) const override;

protected:
	void handleAttackInput() override;
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;

	sf::Time m_jumpStartTime = sf::Time::Zero;
	sf::Time m_currentJumpStartTime = sf::Time::Zero;
	sf::Time m_fightStartTime = sf::Time::Zero;
	sf::Time m_currentFightStartTime = sf::Time::Zero;
};