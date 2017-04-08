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
	void setDead() override;
	
	sf::Time getConfiguredWaitingTime() const override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Jeremy; }

	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;

	void notifyMorgianaDeath(const sf::Vector2f& newPos);
	void notifyRoyDeath(const sf::Vector2f& newPos);

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	// components
	void loadComponents();
	ParticleComponent* m_pc;
	LightComponent* m_light;
};