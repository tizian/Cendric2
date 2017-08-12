#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;
class LightComponent;

class YashaBoss final : public Boss {
public:
	YashaBoss(const Level* level, Screen* screen);

	float getConfiguredDistanceToHPBar() const override;
	sf::Time getConfiguredWaitingTime() const override;
	void setPosition(const sf::Vector2f& pos) override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Yasha; }

protected:
	std::string getSpritePath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	// particles & light
	LightComponent* m_lc;
	ParticleComponent* m_pc;
	particles::AimedVelocityGenerator* m_velGen = nullptr;
	void loadComponents();
};