#pragma once

#include "global.h"
#include "Spells/Spell.h"

// not a spell projectile but it controls its mob. it is invisible for now (no texture), but that could change.
class AntiGravitySpell final : public Spell {
public:
	AntiGravitySpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	bool getConfiguredRotateSprite() const override;
	void setDisposed() override;
};