#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class TorchTile;
class JanusBossMovingBehavior;

class JanusBossMask {
public:
	JanusBossMask();

	void setPosition(const sf::Vector2f& pos);
	void setEvil(bool isEvil);
	void render(sf::RenderTarget& target);

private:
	void load();
	std::string getSpritesheetPath() const;

private:
	sf::Sprite m_sprite;
};

enum JanusBossPhase {
	Chop,
	Vulnerable,
	ToTorch,
	TorchSpell1,
	TorchSpell2
}; 

class JanusBoss : virtual public Boss {
public:
	JanusBoss(const Level* level, Screen* screen);
	virtual ~JanusBoss();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	float getConfiguredDistanceToHPBar() const override;
	sf::Time getConfiguredWaitingTime() const override;
	void setPosition(const sf::Vector2f& pos) override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Janus; }

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
	void setEvil(bool evil);

	// particles
	void loadCloudParticles();
	static const std::string PARTICLE_TEX_PATH;
	
	particles::TextureParticleSystem* m_cloudPs = nullptr;
	particles::ParticleSpawner* m_cloudSpawner = nullptr;

private:
	sf::Time m_timeUntilPhase;
	static const sf::Time PHASE_TIME;
	bool m_hasLitTorches;

	void updateTorchStatus(const sf::Time& frameTime);
	bool m_isBlueTorchesFine;
	bool m_isRedTorchesFine;

	std::vector<TorchTile*> m_blueTorchTiles;
	std::vector<TorchTile*> m_redTorchTiles;

	bool m_torchesLoaded = false;
	void loadTorches();
	sf::Vector2f m_torchSpellPosition;

	JanusBossPhase m_phase;

private:
	JanusBossMask m_mask;
};