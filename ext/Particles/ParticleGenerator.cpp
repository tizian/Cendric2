#include "Particles/ParticleGenerator.h"

#include "Particles/ParticleSystem.h"

#include <cmath>

namespace particles
{
	#ifndef M_PI
	#define M_PI 3.14159265358979323846f
	#endif

	#ifndef DEG_TO_RAD
	#define DEG_TO_RAD M_PI / 180.0f
	#endif

	inline float randomFloat(float low, float high) {
		return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
	}

	inline int randomInt(int low, int high) {
		return (rand() % static_cast<int>(high - low + 1)) + low;
	}

	inline sf::Color randomColor(const sf::Color &low, const sf::Color &high) {
		sf::Uint8 r, g, b, a;

		if (high.r <= low.r) {
			r = high.r;
		}
		else {
			r = (rand() % static_cast<int>(high.r - low.r + 1)) + low.r;
		}

		if (high.g <= low.g) {
			g = high.g;
		}
		else {
			g = (rand() % static_cast<int>(high.g - low.g + 1)) + low.g;
		}

		if (high.b <= low.b) {
			b = high.b;
		}
		else {
			b = (rand() % static_cast<int>(high.b - low.b + 1)) + low.b;
		}

		if (high.a <= low.a) {
			a = high.a;
		}
		else {
			a = (rand() % static_cast<int>(high.a - low.a + 1)) + low.a;
		}

		return { r, g, b, a };
	}

	inline sf::Vector2f randomVector2f(const sf::Vector2f &low, const sf::Vector2f &high) {
		float y = low.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.y - low.y)));
		float x = low.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.x - low.x)));

		return { x, y };
	}


	void PointPositionGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->pos[i] = center;
		}
	}

	void BoxPositionGenerator::generate(ParticleData *data, int startId, int endId) {
		sf::Vector2f posMin{ center.x - size.x, center.y - size.y };
		sf::Vector2f posMax{ center.x + size.x, center.y + size.y };

		for (int i = startId; i < endId; ++i) {
			data->pos[i] = randomVector2f(posMin, posMax);
		}
	}


	void CirclePositionGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			data->pos[i] = { center.x + radius.x * std::sin(phi), center.y + radius.y * std::cos(phi) };
		}
	}

	void DiskPositionGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			float r = randomFloat(0.0f, radius);
			data->pos[i] = { center.x + r * std::sin(phi), center.y + r * std::cos(phi) };
		}
	}


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


	void VelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->vel[i] = randomVector2f(minStartVel, maxStartVel);
		}
	}


	void AngledVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = DEG_TO_RAD * (randomFloat(minAngle, maxAngle) - 90.0f);		// offset to start at top instead of "mathematical 0 degrees"
			sf::Vector2f dir{ std::cos(phi), std::sin(phi) };
			float len = randomFloat(minStartVel, maxStartVel);
			data->vel[i] = dir * len;
		}
	}


	void TimeGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->time[i].x = data->time[i].y = randomFloat(minTime, maxTime);
			data->time[i].z = 0.0f;
		}
	}


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
