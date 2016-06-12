#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
the following skins are available:
0: holy fire (default)
1: shadow fire
*/
class HolyFireSpell : public Spell {
public:
	HolyFireSpell() {};
	virtual ~HolyFireSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	bool m_hasDamaged = false; // this spell only applies its dot once.
	sf::Time m_emitTime = sf::seconds(1.f);

	particles::TextureParticleSystem* m_ps = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};