#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class LightComponent;

class ShackleSprite final : public MovableGameObject {
public:
	ShackleSprite() {};
	~ShackleSprite() {};

	void load(float angle, const sf::Vector2f& pos);
	void gotoMainchar(GameObject* mainChar);

	GameObjectType getConfiguredType() const override { return _Spell; }

private:
	static const float SPEED;
};

// this spell is solely used by the boss: Velius
class ShackleSpell final : public Spell {
public:
	ShackleSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	void loadComponents();

	sf::Time m_timeUntilShackleStart;
	sf::Time m_timeShackleOut;
	sf::Time m_timeShackleIn;
	sf::Time m_timeShackleDark;
	sf::Time m_timeShackleDone;

	std::vector<ShackleSprite*> m_shackleSprites;
	const Level* m_level = nullptr;
	float m_currentDimming = 0.f;
	LightComponent* m_lightComponent;
};