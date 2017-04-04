#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class ParticleComponent;
class LightComponent;

class JeremyBoss : virtual public Enemy {
public:
	JeremyBoss(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;
	
	sf::Time getConfiguredWaitingTime() const override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Jeremy; }

	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;

protected:
	std::string getSpritePath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	// particles
	void loadParticleSystem();
	ParticleComponent* m_ps;
	LightComponent* m_light;
};