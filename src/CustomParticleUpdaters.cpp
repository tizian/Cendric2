#include "CustomParticleUpdaters.h"

#include "Level/DynamicTiles/FluidTile.h"

namespace particles
{
	void FluidUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		const sf::FloatRect *bb = fluidTile->getBoundingBox();

		for (int i = 0; i < endId; ++i) {
			float posX = data->pos[i].x;
			float y = fluidTile->getHeight(posX);

			if (data->pos[i].y + 0.5f * data->size[i].y > bb->top + bb->height - y) {
				fluidTile->splash(nullptr, posX, 0.f, data->vel[i], 0.1f, 0.0f);
				data->kill(i);
				endId = data->countAlive;
			}
		}
	}
}
