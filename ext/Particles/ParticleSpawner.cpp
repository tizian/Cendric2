#include "Particles/ParticleSpawner.h"

#include "Particles/ParticleData.h"
#include "Particles/ParticleHelpers.h"

namespace particles {

void PointSpawner::spawn(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->pos[i] = center;
	}
}

void BoxSpawner::spawn(ParticleData *data, int startId, int endId) {
	float sx = 0.5f * size.x;
	float sy = 0.5f * size.y;
	sf::Vector2f posMin{ center.x - sx, center.y - sy };
	sf::Vector2f posMax{ center.x + sx, center.y + sy };

	for (int i = startId; i < endId; ++i) {
		data->pos[i] = randomVector2f(posMin, posMax);
	}
}

void CircleSpawner::spawn(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float phi = randomFloat(0.0f, M_PI * 2.0f);
		data->pos[i] = { center.x + radius.x * std::sin(phi), center.y + radius.y * std::cos(phi) };
	}
}

void DiskSpawner::spawn(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float phi = randomFloat(0.0f, M_PI * 2.0f);
		float r = randomFloat(0.0f, radius);
		data->pos[i] = { center.x + r * std::sin(phi), center.y + r * std::cos(phi) };
	}
}

}