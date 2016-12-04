#include "CustomParticleUpdaters.h"

#include "Level/DynamicTiles/FluidTile.h"

#include "Particles/ParticleData.h"

namespace particles
{
	void FluidUpdater::update(ParticleData* data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		const sf::FloatRect* bb = fluidTile->getBoundingBox();

		for (int i = 0; i < endId; ++i) {
			float y = fluidTile->getHeight(data->pos[i].x);
			if (data->pos[i].y + 0.5f * data->size[i].y > bb->top + bb->height - y) {
				data->kill(i);
				endId = data->countAlive;
			}
		}
	}

	void FadingColorUpdater::update(ParticleData* data, float dt) {
		const int endId = data->countAlive;
		float alphaScale = 1.f;

		if (m_isFading) {
			m_timeToFade = std::max(0.f, m_timeToFade - dt);
			alphaScale = m_timeToFade / m_initialTimeToFade;
		}

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			sf::Color col = lerpColor(data->startCol[i], data->endCol[i], a);
			col.a = static_cast<sf::Uint8>(col.a * alphaScale);
			data->col[i] = col;
		}
	}

	void FadingColorUpdater::setFading(float timeToFade) {
		m_isFading = true;
		m_initialTimeToFade = timeToFade;
		m_timeToFade = timeToFade;
	}

	void FadingColorUpdater::resetColor() {
		m_isFading = false;
	}
}


