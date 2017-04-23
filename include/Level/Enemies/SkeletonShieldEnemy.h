#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "GameObjectComponents/LightComponent.h"

class SkeletonShieldEnemy final : public Enemy {
public:
	SkeletonShieldEnemy(const Level* level, Screen* screen);

	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;
	void onHit(Spell* spell) override;
	void setDead() override;

	EnemyID getEnemyID() const override { return EnemyID::Skeleton_Shield; }
	float getConfiguredDistanceToHPBar() const override { return 40.f; }

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

	LightComponent* m_lightComponent = nullptr;
	bool m_isHiding = false;
	void initializeHiding(bool asAlly);
};