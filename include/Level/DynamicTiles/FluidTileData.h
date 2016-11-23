#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Logger.h"

enum class FluidTileType {
	VOID,
	Water,
	Lava,
	ShallowWater,
	Poison,
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
	std::string soundPath;

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
	"res/sound/tile/water_splash.ogg"
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
	""
};

const FluidTileData SHALLOW_WATER
{
	FluidTileType::Water,
	1.1f,
	0.1f,
	1.0f,
	20.f,
	0.5f,
	sf::Color(20, 50, 100, 128),
	false,
	false,
	"res/sound/tile/water_splash.ogg"
};

const FluidTileData POISON
{
	FluidTileType::Poison,
	1.1f,
	0.0f,
	1.0f,
	40.f,
	1.f,
	sf::Color(50, 100, 20, 128),
	true,
	true,
	"res/sound/tile/water_splash.ogg"
};