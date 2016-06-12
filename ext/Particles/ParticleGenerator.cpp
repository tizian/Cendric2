#include "Particles/ParticleGenerator.h"

#include "Particles/ParticleData.h"
#include "Particles/ParticleHelpers.h"

namespace particles {

/* Size Generators */

void SizeGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float startSize = randomFloat(minStartSize, maxStartSize);
		float endSize = randomFloat(minEndSize, maxEndSize);
		data->size[i].x = data->size[i].y = startSize;
		data->size[i].z = endSize;
	}
}

void ConstantSizeGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->size[i].x = data->size[i].y = data->size[i].z = size;
	}
}


/* Rotation Generators */

void RotationGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float startPhi = DEG_TO_RAD * (randomFloat(minStartAngle, maxStartAngle));
		float endPhi = DEG_TO_RAD * (randomFloat(minEndAngle, maxEndAngle));
		data->angle[i].x = data->angle[i].y = startPhi;
		data->angle[i].z = endPhi;
	}
}

void ConstantRotationGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float phi = DEG_TO_RAD * (angle);
		data->angle[i].x = data->angle[i].y = data->angle[i].z = phi;
	}
}

void DirectionDefinedRotationGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float phi = 0.5f * M_PI - std::atan2(-data->vel[i].y, data->vel[i].x);
		data->angle[i].x = data->angle[i].y = data->angle[i].z = phi;
	}
}


/* Color Generators */

void ColorGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->startCol[i] = randomColor(minStartCol, maxStartCol);
		data->endCol[i] = randomColor(minEndCol, maxEndCol);
	}
}

void ConstantColorGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->startCol[i] = data->endCol[i] = color;
	}
}


/* Velocity Generators */

void VelocityGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->vel[i] = randomVector2f(minStartVel, maxStartVel);
	}
}

void AngledVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		float phi = DEG_TO_RAD * (randomFloat(minAngle, maxAngle) - 90.0f);		// offset to start at top instead of "mathematical 0 degrees"
		sf::Vector2f dir{ std::cos(phi), std::sin(phi) };
		float len = randomFloat(minStartSpeed, maxStartSpeed);
		data->vel[i] = dir * len;
	}
}

void AimedVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		sf::Vector2f dir = goal - data->pos[i];
		float magnitude = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		dir /= magnitude;
		float len = randomFloat(minStartSpeed, maxStartSpeed);
		data->vel[i] = dir * len;
	}
}


/* Time Generators */

void TimeGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->time[i].x = data->time[i].y = randomFloat(minTime, maxTime);
		data->time[i].z = 0.0f;
	}
}


/* Texture Coordinates Generators */

void TexCoordsGenerator::generate(ParticleData *data, int startId, int endId) {
	for (int i = startId; i < endId; ++i) {
		data->texCoords[i] = texCoords;
		data->frame[i] = 0;
		data->frameTimer[i] = 0.f;
	}
}

void TexCoordsRandomGenerator::generate(ParticleData *data, int startId, int endId) {
	int low = 0;
	int high = static_cast<int>(texCoords.size() - 1);
	if (high < low) return;
	for (int i = startId; i < endId; ++i) {
		int idx = randomInt(low, high);
		data->texCoords[i] = texCoords[idx];
		data->frame[i] = idx;
		data->frameTimer[i] = 0.f;
	}
}

}