#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LightSpell : public Spell {
public:
	LightSpell() {};
	virtual ~LightSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	particles::TextureParticleSystem* m_ps = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};