#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class ParticleComponent;

namespace particles {
	class LineSpawner;
	class AngledVelocityGenerator;
}

// skins:
// 0: damage
// 1: heal
// 2: protection
class YashaBossAdd final : public Enemy {
public:
	YashaBossAdd(const Level* level, Screen* screen);

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void update(const sf::Time& frameTime) override;
	void setDead() override;
	void setBoss(LevelMovableGameObject* boss);
	void revive();

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	EnemyID getEnemyID() const override { return EnemyID::YashaAdd; }
	int getMentalStrength() const override { return 4; }

private:
	std::string getSpritePath() const override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadComponents();
	void loadLineParticles();
	void loadSpells() override {};
	void loadAnimation(int skinNr) override;

	ParticleComponent* m_pc;
	ParticleComponent* m_spellPc;
	particles::LineSpawner* m_lineSpawner;
	particles::AngledVelocityGenerator* m_lineVelGen;
	LevelMovableGameObject* m_boss;
	sf::Time m_hotDotTick = sf::Time::Zero;
};