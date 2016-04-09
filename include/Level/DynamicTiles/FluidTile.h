#pragma once

#include "Level/LevelDynamicTile.h"

#include "global.h"
#include "Particles/ParticleSystem.h"
#include "Level/DynamicTiles/FluidTileData.h"

class FrozenWaterTile;
class MovableGameObject;

struct FluidColumn {
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

class FluidTile : public LevelDynamicTile {
public:
	FluidTile(LevelScreen* levelScreen);
	~FluidTile();
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	float getHeight(float xPosition) const;

	void splash(const MovableGameObject* source, float xPosition, float velocity);
	void splash(const MovableGameObject* source, float xPosition, float width, float velocity);

	void freeze(int index);
	void melt(int index);

	bool isFrozen(int index);

private:
	void checkForMovableTiles();

private:
	FluidTileData m_data;
	float m_x, m_y;
	float m_width, m_height;
	int m_nTiles;		// number subtiles (horizontally)

	int m_nColumns;
	std::vector<FluidColumn> m_columns;

	float *m_leftDeltas;
	float *m_rightDeltas;

	sf::VertexArray m_vertexArray;

	std::vector<FrozenWaterTile *> m_frozenTiles;

	std::unique_ptr<particles::MetaballParticleSystem> m_ps;
	sf::Vector2f *m_particlePosition = nullptr;
	float *m_particleMinSpeed = nullptr;
	float *m_particleMaxSpeed = nullptr;

	std::map<const MovableGameObject*, sf::Sound*> m_soundMap;

public:
	static const float	SURFACE_THICKNESS;
	static const int	NUMBER_COLUMNS_PER_SUBTILE;
};