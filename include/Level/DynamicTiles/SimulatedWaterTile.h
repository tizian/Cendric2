#pragma once

#include "Level/LevelDynamicTile.h"

#include "global.h"
#include "Particles/ParticleSystem.h"
#include "Level/DynamicTiles/SimulatedWaterTileData.h"

class FrozenWaterTile;

struct WaterColumn {
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

// tizs awesome water
class SimulatedWaterTile : public LevelDynamicTile {
public:
	SimulatedWaterTile(Level* level) : LevelDynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	float getHeight(float xPosition);

	void splash(float xPosition, float velocity);
	void splash(float xPosition, float width, float velocity);

	void freeze(int index);
	void melt(int index);

	bool isFrozen(int index);

private:
	SimulatedWaterTileData m_data;
	float m_x, m_y;
	float m_width, m_height;
	int m_nTiles;		// number tiles (width)

	int m_nColumns;
	std::vector<WaterColumn> m_columns;

	float *m_leftDeltas;
	float *m_rightDeltas;

	sf::VertexArray m_vertexArray;

	std::vector<FrozenWaterTile *> m_frozenTiles;

	std::unique_ptr<particles::MetaballParticleSystem> m_ps;
	sf::Vector2f *m_particlePosition = nullptr;
	float *m_particleMinSpeed = nullptr;
	float *m_particleMaxSpeed = nullptr;

	sf::Sound m_sound;

public:

	static const float WATER_SURFACE_THICKNESS;
	static const int NUMBER_COLUMNS_PER_SUBTILE;
};