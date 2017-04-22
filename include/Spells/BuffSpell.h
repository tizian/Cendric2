#pragma once

#include "global.h"
#include "Spells/Spell.h"

class BuffSpell final : public virtual Spell {
public:
	BuffSpell(const AttributeData& buff);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	AttributeData m_buff;
};