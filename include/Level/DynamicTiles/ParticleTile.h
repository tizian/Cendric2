#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "World/LightObject.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

// available skins:
// 1: fire
// 2: blue fire
// 3: toxic water
// 4: green fire
// 5: green embers
// 6: purple embers
class ParticleTile final : public LevelDynamicTile {
public:
	ParticleTile(LevelScreen* levelScreen);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Particle; }

public:
	static particles::ColorGenerator* getWaterColorGenerator(const std::string& color);
	static particles::ColorGenerator* getEmberColorGenerator(const std::string& color);
	static particles::ColorGenerator* getFlameColorGenerator(const std::string& color);

private:
	bool m_isForegroundTile = true;
	std::string m_color;
	ParticleComponent* m_pc;
	particles::AimedVelocityGenerator* m_velGen = nullptr;
	sf::RenderTexture* getParticleTexture();
	

	void loadFlameParticles();
	void loadWaterParticles();
	void loadEmberParticles();
	void loadParticleSystem(int skinNr);
};