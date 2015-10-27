#pragma once

#include "global.h"
#include "Spell.h"

class LightSpell : public Spell {
public:
	LightSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;
	void setDisposed() override;

private:
	LightObject* m_lightObject = nullptr;
};