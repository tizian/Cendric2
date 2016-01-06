#pragma once

#include "global.h"
#include "Spell.h"

class DivineShieldSpell : public Spell {
public:
	DivineShieldSpell(const AttributeData& additionalResistance);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;
	void execOnHit(LevelMovableGameObject* target) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;
	void setDisposed() override;

private:
	AttributeData m_additionalResistance;
	LightObject* m_lightObject;
};