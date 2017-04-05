#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

enum class WardenState {
	Idle,
	Observing,
	Triggered
};

// A warden that will kill the main character on sight.
class WardenEnemy : public virtual Enemy {
public:
	WardenEnemy(const Level* level, Screen* screen);
	virtual ~WardenEnemy() {}

	void update(const sf::Time& frameTime) override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	int getMentalStrength() const override { return 4; }
	EnemyID getEnemyID() const override { return EnemyID::Warden; }
	WardenState getWardenState() const { return m_wardenState; }
	
protected:
	std::string getSpritePath() const override;
	virtual std::string getParticleTexture() const;

	void loadAttributes() override;
	void loadSpells() override {};
	void loadAnimation(int skinNr) override;

	virtual void loadComponents();

	float m_observedRange;
	WardenState m_wardenState = WardenState::Idle;

	ParticleComponent* m_pc;
	particles::ColorGenerator* m_colGen;

	static const float SPEED_IDLE;
	static const float SPEED_CHASING;
};