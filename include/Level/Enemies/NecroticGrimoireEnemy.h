#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"
#include "Level/Enemies/BookEnemy.h"

class DivineTorchTile;

// A flying necrotic grimoire that needs 
// three burning divine torches to be attackable
class NecroticGrimoireEnemy final : public BookEnemy {
public:
	NecroticGrimoireEnemy(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;
	
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	EnemyID getEnemyID() const override { return EnemyID::NecroticGrimoire; }
	void setDead() override;
	int getMentalStrength() const override { return 4; };
	
protected:
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

private:
	bool m_hasLoadedTorches = false;
	void loadTorches();

	std::vector<DivineTorchTile*> m_torches;
	static const float TORCH_DISTANCE;
	static const int ACTIVE_TORCH_COUNT;
	static const sf::Time SUMMON_TIME;

	sf::Time m_summonTime = sf::Time::Zero;
	void summonEnemy();
};