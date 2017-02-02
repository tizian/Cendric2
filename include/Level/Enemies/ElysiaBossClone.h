#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class ElysiaBossClone : public Enemy {
public:
	ElysiaBossClone(const Level* level, Screen* screen);
	~ElysiaBossClone();

	void update(const sf::Time& frameTime) override;

	int getMentalStrength() const override;
	float getConfiguredDistanceToHPBar() const override;
	sf::Time getConfiguredWaitingTime() const override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Elysia; }

protected:
	std::string getSpritePath() const override;
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	bool hasReachedMainchar;
};