#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class MorgianaBoss : virtual public Enemy {
public:
	MorgianaBoss(const Level* level, Screen* screen);

	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

	sf::Time getConfiguredWaitingTime() const override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Morgiana; }

	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;
	
	void notifyJeremyDeath();
	void notifyRoyDeath();

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	bool m_isBlocking = false;
	sf::Time m_blockingTime = sf::Time::Zero;
	static const sf::Time BLOCKING_TIME;
};