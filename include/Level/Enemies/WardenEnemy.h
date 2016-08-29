#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

// A warden that will kill the main character on sight.
class WardenEnemy : public virtual Enemy {
public:
	WardenEnemy(const Level* level, Screen* screen);
	~WardenEnemy() {}

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	int getMentalStrength() const override;
	EnemyID getEnemyID() const override { return EnemyID::Warden; }
	
protected:
	std::string getSpritePath() const override;

	void loadAttributes() override;
	void loadSpells() override {};
	void loadAnimation(int skinNr) override;

	void loadParticleSystem();
	void updateParticleSystem(const sf::Time& frameTime);

private:
	float m_observedRange;

	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	static const float SPEED_IDLE;
	static const float SPEED_CHASING;
};