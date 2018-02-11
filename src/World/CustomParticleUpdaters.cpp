#include "World/CustomParticleUpdaters.h"

#include "Level/DynamicTiles/FluidTile.h"
#include "Level/LevelMovableGameObject.h"
#include "Level/Level.h"

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

	void AimedCircleVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			float rho = randomFloat(0.0f, 1.0f);
			float x = std::sqrt(rho) * std::cos(phi) * goalRadius;
			float y = std::sqrt(rho) * std::sin(phi) * goalRadius;

			sf::Vector2f dir = goal + sf::Vector2f(x, y) - data->pos[i];
			float magnitude = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			dir /= magnitude;
			float len = randomFloat(minStartSpeed, maxStartSpeed);
			data->vel[i] = dir * len;
		}
	}

	AttractingEulerUpdater::AttractingEulerUpdater(const sf::Vector2f* refPos, float fraction) {
		assert(refPos);
		m_refPos = refPos;
		m_fraction = clamp(fraction, 0.f, 1.f);
		m_oldPos = *m_refPos;
	}

	void AttractingEulerUpdater::update(ParticleData *data, float dt) {
		auto diffPos = *m_refPos - m_oldPos;
		m_oldPos = *m_refPos;

		for (int i = 0; i < data->countAlive; ++i) {
			data->acc[i] += globalAcceleration;
			data->vel[i] += dt * data->acc[i];
			data->pos[i] += dt * data->vel[i] + m_fraction * diffPos;
		}
	}

	KillingUpdater::KillingUpdater(LevelMovableGameObject* mob) : m_mob(mob) {
		m_mob = mob;
	}

	void KillingUpdater::update(ParticleData *data, float dt)  {
		int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			if (m_mob->getBoundingBox()->contains(data->pos[i])) {
				m_mob->setDead();
			}
		}
	}

	CollidingUpdater::CollidingUpdater(const Level* level) : m_level(level) {};

	void CollidingUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			if (m_level->collidesWithCollidableLayer(data->pos[i])) {
				data->kill(i);
				endId = data->countAlive;
			}
		}
	}

	void EllipseSpawner::spawn(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.f, M_PI * 2.0f);
			float rho = randomFloat(0.f, 1.f);
			float x = std::sqrt(rho) * std::cos(phi) * radius.x;
			float y = std::sqrt(rho) * std::sin(phi) * radius.y;
			data->pos[i] = { center.x + x, center.y + y };
		}
	}

	void LineSpawner::spawn(ParticleData *data, int startId, int endId) {
		const sf::Vector2f a = point2 - point1;
		for (int i = startId; i < endId; ++i) {
			float x = randomFloat(0.f, 1.f);
			data->pos[i] = a * x + point1;
		}
	}
}


