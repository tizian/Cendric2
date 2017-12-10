#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Screens/Screen.h"

class ParticleComponent;

// skins:
// 0: koray
// 1: robert
// 2: inina
class VeliusVictim final : public Enemy {
public:
	VeliusVictim(const Level* level, Screen* screen);

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void update(const sf::Time& frameTime) override;
	void setDead() override;
	void callToDie();

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	EnemyID getEnemyID() const override { return EnemyID::VeliusVictim; }
	int getMentalStrength() const override { return 4; }

private:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override {};
	void loadAnimation(int skinNr) override;

	sf::Time m_timeUntilDamage;

	void loadDeathParticles();
	ParticleComponent* m_deathPc;

	sf::Time m_fadingTime = sf::seconds(2.f);
	sf::Time m_particleTime = sf::seconds(2.f);
};