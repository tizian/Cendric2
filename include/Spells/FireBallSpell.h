#pragma once

#include "global.h"
#include "Spell.h"
#include "LightObject.h"

class FireBallSpell : public Spell
{
public:
	FireBallSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	void setPosition(const sf::Vector2f& pos) override;	
	void setScreen(Screen* screen) override;
	void setDisposed() override;

private:
	LightObject* m_lightObject;
	sf::Sound m_sound;
};