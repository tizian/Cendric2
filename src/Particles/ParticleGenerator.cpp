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

	inline float randomFloat(float low, float high)
	{
		return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	}

	inline int randomInt(int low, int high)
	{
		return (rand() % (int)(high - low + 1)) + low;
	}

	inline sf::Color randomColor(const sf::Color &low, const sf::Color &high)
	{
		sf::Uint8 r = (rand() % (int)(high.r - low.r + 1)) + low.r;
		sf::Uint8 g = (rand() % (int)(high.g - low.g + 1)) + low.g;
		sf::Uint8 b = (rand() % (int)(high.b - low.b + 1)) + low.b;
		sf::Uint8 a = (rand() % (int)(high.a - low.a + 1)) + low.a;

		return sf::Color(r, g, b, a);
	}

	inline sf::Vector2f randomVector2f(const sf::Vector2f &low, const sf::Vector2f &high)
	{
		float x = low.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high.x - low.x)));
		float y = low.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high.y - low.y)));

		return sf::Vector2f(x, y);
	}


	void PointPositionGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->pos[i] = center;
		}
	}

	void BoxPositionGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		sf::Vector2f posMin{ center.x - size.x, center.y - size.y };
		sf::Vector2f posMax{ center.x + size.x, center.y + size.y };

		for (size_t i = startId; i < endId; ++i)
		{
			p->pos[i] = randomVector2f(posMin, posMax);
		}
	}


	void CirclePositionGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			p->pos[i] = { center.x + radius.x * std::sin(phi), center.y + radius.y * std::cos(phi) };
		}
	}

	void DiskPositionGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			float r = std::sqrt(randomFloat(0.0f, radius));
			p->pos[i] = { center.x + r * std::sin(phi), center.y + r * std::cos(phi) };
		}
	}


	void SizeGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			float startSize = randomFloat(minStartSize, maxStartSize);
			float endSize = randomFloat(minEndSize, maxEndSize);
			p->size[i].x = p->size[i].y = startSize;
			p->size[i].z = endSize;
		}
	}


	void ConstantSizeGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->size[i].x = p->size[i].y = p->size[i].z = size;
		}
	}


	void ColorGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->startCol[i] = randomColor(minStartCol, maxStartCol);
			p->endCol[i] = randomColor(minEndCol, maxEndCol);
		}
	}


	void ConstantColorGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->startCol[i] = p->endCol[i] = color;
		}
	}


	void VelocityGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->vel[i] = randomVector2f(minStartVel, maxStartVel);
		}
	}


	void AngledVelocityGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			float phi = DEG_TO_RAD * (randomFloat(minAngle, maxAngle) - 90.0f);		// offset to start at top instead of "mathematical 0 degrees"
			sf::Vector2f dir{ std::cos(phi), std::sin(phi) };
			float len = randomFloat(minStartVel, maxStartVel);
			p->vel[i] = dir * len;
		}
	}


	void TimeGenerator::generate(ParticleData *p, size_t startId, size_t endId)
	{
		for (size_t i = startId; i < endId; ++i)
		{
			p->time[i].x = p->time[i].y = randomFloat(minTime, maxTime);
			p->time[i].z = (float)0.0;
		}
	}
}
