#include "CustomParticleUpdaters.h"

#include "Level/DynamicTiles/FluidTile.h"

namespace particles
{
	void FluidUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		const sf::FloatRect *bb = fluidTile->getBoundingBox();

		for (int i = 0; i < endId; ++i) {
			float y = fluidTile->getHeight(data->pos[i].x);
			if (data->pos[i].y + 0.5f * data->size[i].y > bb->top + bb->height - y) {
				data->kill(i);
				endId = data->countAlive;
			}
		}
	}
}
