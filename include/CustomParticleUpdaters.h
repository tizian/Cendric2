#pragma once

#include "Particles/ParticleSystem.h"

class SimulatedWaterTile;

namespace particles
{
	class SimulatedWaterUpdater : public ParticleUpdater {
	public:
		SimulatedWaterUpdater() {}
		~SimulatedWaterUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		SimulatedWaterTile *water = nullptr;
	};
}
