#pragma once

#include "global.h"
#include "Spell.h"

class DivineShieldSpell : public Spell {
public:
	DivineShieldSpell(int additionalResistance);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;
	void setDisposed() override;

private:
	AttributeData m_additionalResistance;
	LightObject* m_lightObject;
};