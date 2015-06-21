#pragma once

#include "global.h"
#include "DynamicTile.h"

struct WaterColumn
{
	float targetHeight;
	float height;
	float velocity;

	void update(float damping, float tension, float dt)
	{
		float x = targetHeight - height;
		float a = tension * x - damping * velocity;

		velocity += a * dt;
		height += velocity * dt;
	}
};

// tizs awesome water
class SimulatedWaterTile : public DynamicTile
{
public:
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void splash(float xPosition, float velocity);

private:
	float m_x, m_y;
	float m_width, m_height;
	int m_nTiles;		// number tiles (width)
	
	int m_nColumns;
	std::vector<WaterColumn> m_columns;

	float *m_leftDeltas;
	float *m_rightDeltas;

	sf::VertexArray m_vertexArray;

	static const float TENSION;
	static const float DAMPING;
	static const float SPREAD;
};