#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "Enums/ResourceID.h"

enum class FluidTileType {
	VOID,
	Water,
	Lava,
	ShallowWater,
	MAX
};

// parameters for the behavior of the fluid tile
struct FluidTileData {
	FluidTileType type;
	float tension;
	float damping;
	float spread;
	float height;
	float velocityScale;
	sf::Color color;
	bool isDeadly;
	bool isFreezable;
	ResourceID sound;

	static FluidTileData getData(int skinNr);
};

const FluidTileData WATER
{
	FluidTileType::Water,
	1.1f,
	0.0f,
	1.0f,
	40.f,
	1.f,
	sf::Color(20, 50, 100, 128),
	false,
	true,
	ResourceID::Sound_tile_water
};

const FluidTileData LAVA
{
	FluidTileType::Lava,
	0.6f,
	0.10f,
	0.4f,
	40.f,
	1.f,
	sf::Color(245, 69, 10, 128),
	true,
	false,
	ResourceID::VOID
};

const FluidTileData SHALLOW_WATER
{
	FluidTileType::Water,
	0.4f,
	0.15f,
	0.2f,
	25.f,
	0.4f,
	sf::Color(20, 50, 100, 128),
	false,
	false,
	ResourceID::Sound_tile_water
};