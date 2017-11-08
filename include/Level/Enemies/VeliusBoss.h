#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Screens/Screen.h"

enum VeliusBossState {
	AttackIllusion,
	ExtractDivine,
	AttackDivine,
	ExtractNecromancy,
	AttackNecromancy,
	ExtractTwilight,
	AttackTwilight,
	ExtractElemental
};

class VeliusBoss final : public Boss {
public:
	VeliusBoss(const Level* level, Screen* screen);

	EnemyID getEnemyID() const override { return EnemyID::Boss_Velius; }

	float getConfiguredDistanceToHPBar() const override;

protected:
	std::string getSpritePath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};