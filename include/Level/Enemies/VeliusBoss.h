#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Screens/Screen.h"

class MirrorRay;

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
	~VeliusBoss();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Velius; }
	VeliusBossState getBossState() const { return m_bossState; }

	float getConfiguredDistanceToHPBar() const override;

protected:
	std::string getSpritePath() const override;

	void updateBossState(const sf::Time& frameTime);
	void setBossState(VeliusBossState state);
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

private:
	MirrorRay* m_ray = nullptr;
	VeliusBossState m_bossState;
};