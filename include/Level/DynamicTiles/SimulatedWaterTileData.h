#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "Enums/ResourceID.h"

enum class SimulatedWaterTileType {
	VOID,
	Water,
	Lava,
	MAX
};

// parameters for the behavior of the simulated water tile
struct SimulatedWaterTileData {
	SimulatedWaterTileType type;
	float tension;
	float damping;
	float spread;
	sf::Color color;
	bool isDeadly;
	bool isFreezable;
	ResourceID sound;

	static SimulatedWaterTileData getData(int skinNr);
};

const SimulatedWaterTileData WATER
{
	SimulatedWaterTileType::Water,
	0.4f,
	0.05f,
	0.7f,
	sf::Color(20, 50, 100, 128),
	false,
	true,
	ResourceID::Sound_tile_water
};

const SimulatedWaterTileData LAVA
{
	SimulatedWaterTileType::Lava,
	0.6f,
	0.10f,
	0.4f,
	sf::Color(245, 69, 10, 128),
	true,
	false,
	ResourceID::VOID
};