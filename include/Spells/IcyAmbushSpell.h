#pragma once

#include "global.h"
#include "Spells/Spell.h"

//skins:
//0: icy dagger
//1: shadow ninja star
class IcyAmbushSpell : public Spell {
public:
	IcyAmbushSpell() : Spell() {};
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	void loadComponents();
};