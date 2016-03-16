#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

enum class FallingTileState {
	Idle,
	Falling,
	Returning
};

class FallingTile : public virtual LevelMovableTile {
public:
	FallingTile(Level* level);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void setScreen(Screen* screen) override;

private:
	GameObject* m_mainChar;
	const float GRAVITY_ACCELERATION = 1000.f;
	const float RETURN_VELOCITY = 80.f;
	const float AGGRO_DISTANCE = 100.f;

	float m_startHeight;
	FallingTileState m_tileState;
};