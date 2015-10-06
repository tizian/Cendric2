#include "CustomParticleUpdaters.h"

#include "DynamicTiles/SimulatedWaterTile.h"

namespace particles
{
	void SimulatedWaterUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		const sf::FloatRect *bb = water->getBoundingBox();

		for (int i = 0; i < endId; ++i) {
			float y = water->getHeight(data->pos[i].x);

			if (data->pos[i].y + data->size[i].x > bb->top + bb->height - y - water->WATER_SURFACE_THICKNESS) {
				data->kill(i);
				if (data->countAlive < data->count) {
					endId = data->countAlive;
				}
				else {
					endId = data->count;
				}
			}
		}
	}
}
