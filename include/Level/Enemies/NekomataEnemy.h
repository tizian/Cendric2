#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

// A spooky Nekomata in a level
class NekomataEnemy : public Enemy {
public:
	NekomataEnemy(const Level* level, Screen* screen);
	virtual ~NekomataEnemy();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void loadAnimation() override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	EnemyID getEnemyID() const override { return EnemyID::Nekomata; }

protected:
	std::string getSpritePath() const override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;

	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};