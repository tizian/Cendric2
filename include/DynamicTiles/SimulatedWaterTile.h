#pragma once

#include "global.h"
#include "DynamicTile.h"

class FrozenWaterTile;

struct WaterColumn
{
	float targetHeight;
	float height;
	float velocity;
	bool fixed;

	void update(float damping, float tension, float dt)
	{
		if (fixed) return;
		
		float x = targetHeight - height;
		float a = tension * x - damping * velocity;

		velocity += a * dt;
		height += velocity * dt;
		height = std::max(0.f, height);
	}
};

// tizs awesome water
class SimulatedWaterTile : public DynamicTile
{
public:
	SimulatedWaterTile() : DynamicTile() {}
	void init() override;
	void load(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void splash(float xPosition, float velocity);

	void freeze(int index);
	void melt(int index);

	bool isFrozen(int index);

private:
	float m_x, m_y;
	float m_width, m_height;
	int m_nTiles;		// number tiles (width)
	
	int m_nColumns;
	std::vector<WaterColumn> m_columns;

	float *m_leftDeltas;
	float *m_rightDeltas;

	sf::VertexArray m_vertexArray;

	std::vector<FrozenWaterTile *> m_frozenTiles;

	static const float TENSION;
	static const float DAMPING;
	static const float SPREAD;

	static const float WATER_LEVEL;
	static const float WATER_SURFACE_THICKNESS;
	static const int NUMBER_COLUMNS_PER_SUBTILE;
};