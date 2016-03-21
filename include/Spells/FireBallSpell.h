#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "LightObject.h"

/*
the following skins are available:
0: fiery fireball (default)
1: icy fireball
2: shadow fireball
3: light fireball
*/
class FireBallSpell : public Spell {
public:
	FireBallSpell();
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;

	void setDisposed() override;

private:
	LightObject* m_lightObject;
	sf::Sound m_sound;
};