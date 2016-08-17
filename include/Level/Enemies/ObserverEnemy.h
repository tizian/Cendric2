#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

// An observer that can put the main character in jail. It is invincible and will only attack Cendric if he steals something
class ObserverEnemy : public virtual Enemy {
public:
	ObserverEnemy(const Level* level, Screen* screen);
	~ObserverEnemy() {}

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	// returns whether this observer sees the main char. (called at time of stealing)
	// if it's the first time he was caught stealing, the observer will react with a warning
	bool notifyStealing(bool isFirstTime);
	void loadAnimation() override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	EnemyID getEnemyID() const override { return EnemyID::Observer; }
	
protected:
	std::string getSpritePath() const override;

	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	void loadSpells() override {};

	void loadParticleSystem();
	void updateParticleSystem(const sf::Time& frameTime);

private:
	float m_observedRange;

	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;
	particles::ColorGenerator* m_colGen;

	void setObserverChasing();
	void setObserverIdle();
	void setObserverTriggered();

	static const float SPEED_IDLE;
	static const float SPEED_CHASING;
};