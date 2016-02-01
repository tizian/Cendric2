#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"
#include "Particles/ParticleSystem.h"

class ModifierTile : public LevelDynamicTile {
public:
	ModifierTile(Level* level);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;
	void setDisposed() override;

	void setModifier(const SpellModifier& modififer);

private:
	SpellModifier m_modifier;
	sf::Time m_particleTime = sf::seconds(3.f);

	LightObject* m_lightObject = nullptr;

	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::PointPositionGenerator* m_pointGenerator = nullptr;

	void addModifier();

	void loadParticleSystem();

	void updateParticleSystemPosition();
};