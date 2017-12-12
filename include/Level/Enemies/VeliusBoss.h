#pragma once

#include "global.h"
#include "Level/Boss.h"
#include "Level/Level.h"
#include "Screens/Screen.h"

class MirrorRay;
class ParticleComponent;
class VeliusBossClone;
class VeliusVictim;

enum VeliusBossState {
	AttackIllusion,
	ExtractDivine,
	AttackDivine,
	ExtractNecromancy,
	AttackNecromancy,
	ExtractTwilight,
	AttackTwilight,
	ExtractElemental
};

class VeliusBoss final : public Boss {
public:
	VeliusBoss(const Level* level, Screen* screen);
	~VeliusBoss();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void onHit(Spell* spell) override;

	EnemyID getEnemyID() const override { return EnemyID::Boss_Velius; }
	VeliusBossState getBossState() const { return m_bossState; }

	float getConfiguredDistanceToHPBar() const override;
	bool isBlocked() const;

protected:
	std::string getSpritePath() const override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
	void loadComponents();

private:
	// boss state
	void updateBossState(const sf::Time& frameTime);
	void setBossState(VeliusBossState state);
	void startAttackPhase();
	VeliusBossState m_bossState;

	// blocking
	void updateBlocking(const sf::Time& frameTime);
	void setBlocking();
	void clearBlocking();
	void loadBlockingParticles();
	ParticleComponent* m_blockingBubble;
	bool m_isBlocking = false;
	bool m_isIndefinitelyBlocking = false;
	sf::Time m_timeUntilBlocking;
	sf::Time m_blockingGraceTime;
	sf::Time m_blockingTime;
	static const sf::Time BLOCKING_TIME;
	static const sf::Time BLOCKING_GRACE_TIME;

	// clones
	static const sf::Time CLONE_TIMEOUT;
	static const std::vector<sf::Vector2f> CLONE_LOCATIONS;
	sf::Time m_timeUntilClones;
	void updateClonesStart(const sf::Time& frameTime, int healthThreshold);
	void setupClones(int phase);
	void clearClones();
	bool m_isClonesDoneInPhase = false;
	bool m_isClones = false;

	// shackle
	void updateShackle(const sf::Time& frameTime, int healthThreshold);
	bool m_isShackling = false;
	sf::Time m_timeUntilShackleDone;
	static const sf::Time SHACKLE_DURATION;

	// raycasts
	MirrorRay* m_ray = nullptr;
	bool m_isCalling = false;
	sf::Time m_timeUntilCallingComplete;
	static const sf::Time CALLING_TIME;
	void callToDie(const sf::Color& color);
	void startExtraction(const sf::Color& color);
	void updateExtraction();
	VeliusVictim* m_victim = nullptr;

	// colors
	static const sf::Color V_COLOR_ILLUSION;
	static const sf::Color V_COLOR_TWILIGHT;
	static const sf::Color V_COLOR_NECROMANCY;
	static const sf::Color V_COLOR_DIVINE;
	static const sf::Color V_COLOR_ELEMENTAL;

	// velius' level
	int m_veliusLevel;
};