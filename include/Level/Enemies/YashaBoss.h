#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;
class LightComponent;
class YashaBossMovingBehavior;

enum class YashaBossState {
	Fireballing,
	GotoStartCat,
	StartCat,
	Cat,
	GotoExplosion,
	Explosion
};

class YashaBoss final : public Boss {
	friend class YashaBossMovingBehavior;
public:
	YashaBoss(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& target) override;

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

private:
	void updateBossState(const sf::Time& frameTime);
	YashaBossState m_bossState;
	sf::Time m_timeUntilNextState;
	bool m_fadeIn = true;
	float m_currentDimming = 0.f;
	void updateFading(const sf::Time& frameTime);
	void startBossState(YashaBossState state);
	void spawnCats();
	sf::Sprite m_eyes;

private:
	static const sf::Vector2f ROOM_MID;
	static const float FADE_TIME;
	static const std::vector<sf::Vector2f> ADD_LOCATIONS;
};