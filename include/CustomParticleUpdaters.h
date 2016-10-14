#pragma once

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleHelpers.h"

class FluidTile;

namespace particles
{
	class FluidUpdater : public ParticleUpdater {
	public:
		FluidUpdater() {}
		~FluidUpdater() {}

		void update(ParticleData* data, float dt);

	public:
		FluidTile* fluidTile = nullptr;
	};

	class FadingColorUpdater : public ParticleUpdater {
	public:
		FadingColorUpdater() {}
		~FadingColorUpdater() {}

		void update(ParticleData* data, float dt);

		// start the fading timer (in seconds)
		void setFading(float timeToFade);

	private:
		float m_timeToFade;
		float m_initialTimeToFade;
		bool m_isFading = false;
	};
}
