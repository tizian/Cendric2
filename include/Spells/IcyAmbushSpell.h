#pragma once

#include "global.h"
#include "Spells/Spell.h"

//skins:
//0: icy dagger
//1: shadow ninja star
class IcyAmbushSpell : public Spell {
public:
	IcyAmbushSpell() : Spell() {};
	virtual ~IcyAmbushSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};