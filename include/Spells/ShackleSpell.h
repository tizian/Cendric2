#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class ShackleSprite final : public AnimatedGameObject {
public:
	ShackleSprite() {};
	~ShackleSprite() {};

	GameObjectType getConfiguredType() const override { return GameObjectType::_Spell; }
};

// this spell is used by the boss: Velius
class ShackleSpell final : public Spell {
public:
	ShackleSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	void loadComponents();

	sf::Time m_timeUntilShackleOut;
	sf::Time m_timeUntilShackleIn;

	std::vector<ShackleSprite*> m_shackleSprites;
};