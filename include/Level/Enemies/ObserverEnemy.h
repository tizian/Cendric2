#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"
#include "Level/Enemies/WardenEnemy.h"

// An observer that can put the main character in jail. It is invincible and will only attack Cendric if he steals something
class ObserverEnemy : public virtual WardenEnemy {
public:
	ObserverEnemy(const Level* level, Screen* screen);
	~ObserverEnemy() {}

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	// returns whether this observer sees the main char. (called at time of stealing)
	// if it's the first time he was caught stealing, the observer will react with a warning
	bool notifyStealing(bool isFirstTime);

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	EnemyID getEnemyID() const override { return EnemyID::Observer; }
	
protected:
	std::string getSpritePath() const override;

	void loadAnimation(int skinNr) override;
	void loadParticleSystem() override;
	void updateParticleSystem(const sf::Time& frameTime) override;

private:
	void setObserverChasing();
	void setObserverIdle();
	void setObserverTriggered();
};