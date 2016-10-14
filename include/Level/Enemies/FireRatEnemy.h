#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class WalkingBehavior;

// A nasty fiery rat in a level

// skins
// 1 normal fire rat
// 2 hidden fire rat that only gets vulnerable when first hit with a ice or windgust spell
class FireRatEnemy : public Enemy {
public:
	FireRatEnemy(const Level* level, Screen* screen);
	virtual ~FireRatEnemy();

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void onHit(Spell* spell) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	EnemyID getEnemyID() const override { return EnemyID::FireRat; }

protected:
	std::string getSpritePath() const override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	bool m_isBurning = false;

	// particle system, only active if we got a hidden fire rat
	particles::TextureParticleSystem* m_ps = nullptr;
	particles::AimedVelocityGenerator* m_velGenerator = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;
	LightComponent* m_lightComponent = nullptr;

	void loadParticleSystem();
};