#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LightComponent;
class ParticleComponent;

namespace particles {
	class AimedVelocityGenerator;
}

class TorchTile final : public LevelDynamicTile {
public:
	TorchTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	const std::string& getColor() const;
	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Torch; }

private:
	void loadComponents();
	std::string getSpritePath() const override;
	LightComponent* m_lightComponent;
	ParticleComponent* m_particleComponent;
	particles::AimedVelocityGenerator* m_velGen = nullptr;
	std::string m_color;
};