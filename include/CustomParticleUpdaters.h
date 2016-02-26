#pragma once

#include "Particles/ParticleSystem.h"

class FluidTile;

namespace particles
{
	class FluidUpdater : public ParticleUpdater {
	public:
		FluidUpdater() {}
		~FluidUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		FluidTile *fluidTile = nullptr;
	};
}
