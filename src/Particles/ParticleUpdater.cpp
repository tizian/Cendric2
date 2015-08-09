#include "Particles/ParticleUpdater.h"

#include "DynamicTiles/SimulatedWaterTile.h"

namespace particles
{
	inline float dot(const sf::Vector2f &a, const sf::Vector2f &b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float lerpFloat(float a, float b, float alpha)
	{
		return a * (1.0f - alpha) + b * alpha;
	}

	inline sf::Color lerpColor(const sf::Color &c1, const sf::Color &c2, float alpha)
	{
		sf::Uint8 r = (sf::Uint8)(c1.r * (1.0f - alpha) + c2.r * alpha);
		sf::Uint8 g = (sf::Uint8)(c1.g * (1.0f - alpha) + c2.g * alpha);
		sf::Uint8 b = (sf::Uint8)(c1.b * (1.0f - alpha) + c2.b * alpha);
		sf::Uint8 a = (sf::Uint8)(c1.a * (1.0f - alpha) + c2.a * alpha);

		return sf::Color(r, g, b, a);
	}

	void EulerUpdater::update(float dt, ParticleData *p)
	{
		const unsigned int endId = p->countAlive;

		for (size_t i = 0; i < endId; ++i)
			p->acc[i] += dt * globalAcceleration;

		for (size_t i = 0; i < endId; ++i)
			p->vel[i] += dt * p->acc[i];

		for (size_t i = 0; i < endId; ++i)
			p->pos[i] += dt * p->vel[i];
	}


	void FloorUpdater::update(float dt, ParticleData *p)
	{
		const unsigned int endId = p->countAlive;

		for (size_t i = 0; i < endId; ++i)
		{
			if (p->pos[i].y < floorY)
			{
				sf::Vector2f force = p->acc[i];
				float normalFactor = dot(force, sf::Vector2f(0.0f, 1.0f));
				if (normalFactor < 0.0f)
					force -= sf::Vector2f(0.0f, 1.0f) * normalFactor;

				float velFactor = dot(p->vel[i], sf::Vector2f(0.0f, 1.0f));
				p->vel[i] -= sf::Vector2f(0.0f, 1.0f) * (1.0f + bounceFactor) * velFactor;
				p->acc[i] = force;
			}
		}
	}


	void AttractorUpdater::update(float dt, ParticleData *p)
	{
		const unsigned int endId = p->countAlive;
		const unsigned int numAttractors = m_attractors.size();
		sf::Vector2f off;
		float dist;
		size_t a;

		for (size_t i = 0; i < endId; ++i)
		{
			for (a = 0; a < numAttractors; ++a)
			{
				off.x = m_attractors[a].x - p->pos[i].x;
				off.y = m_attractors[a].y - p->pos[i].y;
				dist = dot(off, off);
				dist = m_attractors[a].z / dist;

				p->acc[i] += off * dist;
			}
		}
	}


	void SizeUpdater::update(float dt, ParticleData *p)
	{
		const unsigned int endId = p->countAlive;

		for (size_t i = 0; i < endId; ++i)
		{
			float a = p->time[i].z;
			p->size[i].x = lerpFloat(p->size[i].y, p->size[i].z, a);
		}
	}


	void ColorUpdater::update(float dt, ParticleData *p)
	{
		const unsigned int endId = p->countAlive;

		for (size_t i = 0; i < endId; ++i)
		{
			float a = p->time[i].z;
			p->col[i] = lerpColor(p->startCol[i], p->endCol[i], a);
		}
	}


	void TimeUpdater::update(float dt, ParticleData *p)
	{
		unsigned int endId = p->countAlive;

		if (endId == 0) return;

		for (size_t i = 0; i < endId; ++i)
		{
			p->time[i].x -= dt;
			p->time[i].z = 1.0f - (p->time[i].x / p->time[i].y);

			if (p->time[i].x < 0.0f)
			{
				p->kill(i);
				endId = p->countAlive < p->count ? p->countAlive : p->count;
			}
		}
	}

	void SimulatedWaterUpdater::update(float dt, ParticleData *p)
	{
		unsigned int endId = p->countAlive;

		if (endId == 0) return;

		const sf::FloatRect *bb = water->getBoundingBox();

		for (size_t i = 0; i < endId; ++i)
		{
			float y = water->getHeight(p->pos[i].x);
			
			if (p->pos[i].y + p->size[i].x > bb->top + bb->height - y - water->WATER_SURFACE_THICKNESS)
			{
				p->kill(i);
				endId = p->countAlive < p->count ? p->countAlive : p->count;
			}
		}
	}
}
