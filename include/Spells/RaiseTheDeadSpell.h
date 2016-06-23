#pragma once

#include "global.h"
#include "Spells/Spell.h"

class RaiseTheDeadSpell : public Spell {
public:
	RaiseTheDeadSpell() : Spell() {};
	virtual ~RaiseTheDeadSpell();

	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

protected:
	bool checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) override;

private:
	particles::TextureParticleSystem* m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};