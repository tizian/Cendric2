#pragma once

#include "Level/LevelDynamicTile.h"

#include "global.h"
#include "Particles/ParticleSystem.h"
#include "Level/DynamicTiles/FluidTileData.h"

class FrozenWaterTile;
class MovableGameObject;

struct FluidColumn final {
	float targetHeight;
	float height;
	float velocity;
	bool fixed;

	void update(float damping, float tension, float dt) {
		if (fixed) return;

		float x = targetHeight - height;
		float a = tension * x - damping * velocity;

		velocity += a * dt;
		height += velocity * dt;
		height = std::max(0.f, height);
	}
};

class FluidTile final : public LevelDynamicTile {
public:
	FluidTile(LevelScreen* levelScreen);
	~FluidTile();
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	float getHeight(float xPosition) const;

	void splash(const MovableGameObject* source, float xPosition, float width, sf::Vector2f velocity, float waveVelocityScale = 1.f, float particleVelocityScale = 1.f);

	void freeze(int index);
	void melt(int index);

	bool isFrozen(int index);

	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Fluid; }

private:
	void checkForMovableTiles();

private:
	FluidTileData m_data;
	float m_x, m_y;
	float m_width, m_height;
	int m_nTiles;		// number subtiles (horizontally)

	int m_nColumns;
	std::vector<FluidColumn> m_columns;

	float* m_leftDeltas;
	float* m_rightDeltas;
	float m_scale;

	sf::VertexArray m_vertexArray;

	std::vector<FrozenWaterTile*> m_frozenTiles;

	particles::MetaballParticleSystem* m_ps;
	sf::Vector2f* m_emitterPosition = nullptr;
	sf::Vector2f* m_emitterSize = nullptr;
	float* m_particleMinSpeed = nullptr;
	float* m_particleMaxSpeed = nullptr;
	
	sf::Time m_timeUntilDamage;
	std::map<const MovableGameObject*, sf::Sound*> m_soundMap;

public:
	static const float	SURFACE_THICKNESS;
	static const int	NUMBER_COLUMNS_PER_SUBTILE;
	
	static const int	FREEZING_DAMAGE_PER_S;
	static const int	TOP_OFFSET;
};