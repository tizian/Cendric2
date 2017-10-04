#pragma once

#include "global.h"
#include "Level/Enemies/RoyalguardBoss.h"

class RoyalguardTBoss final : public virtual RoyalguardBoss {
public:
	RoyalguardTBoss(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Royalguard_T; }

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};