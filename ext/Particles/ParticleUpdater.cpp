#include "Particles/ParticleUpdater.h"

#include <cmath>

namespace particles
{
	inline float dot(const sf::Vector2f &a, const sf::Vector2f &b) {
		return a.x * b.x + a.y * b.y;
	}

	inline float lerpFloat(float a, float b, float alpha) {
		return a * (1.0f - alpha) + b * alpha;
	}

	inline sf::Color lerpColor(const sf::Color &c1, const sf::Color &c2, float alpha) {
		sf::Uint8 r = (sf::Uint8)(c1.r * (1.0f - alpha) + c2.r * alpha);
		sf::Uint8 g = (sf::Uint8)(c1.g * (1.0f - alpha) + c2.g * alpha);
		sf::Uint8 b = (sf::Uint8)(c1.b * (1.0f - alpha) + c2.b * alpha);
		sf::Uint8 a = (sf::Uint8)(c1.a * (1.0f - alpha) + c2.a * alpha);

		return sf::Color(r, g, b, a);
	}

	void EulerUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			data->acc[i] += globalAcceleration;
		}

		for (int i = 0; i < endId; ++i) {
			data->pos[i] += dt * data->vel[i];
		}

		for (int i = 0; i < endId; ++i) {
			data->vel[i] += dt * data->acc[i];
		}
	}


	void HorizontalCollider::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		if (invert) {
			for (int i = 0; i < endId; ++i) {
				if (data->pos[i].x < pos) {
					data->pos[i].x = pos;

					sf::Vector2f acc = data->acc[i];
					data->acc[i] = sf::Vector2f(-acc.x * bounceFactor, acc.y);

					sf::Vector2f vel = data->vel[i];
					data->vel[i] = sf::Vector2f(-vel.x * bounceFactor, vel.y);
				}
			}
		}
		else {
			for (int i = 0; i < endId; ++i) {
				if (data->pos[i].y > pos) {
					data->pos[i].x = pos;

					sf::Vector2f acc = data->acc[i];
					data->acc[i] = sf::Vector2f(-acc.x * bounceFactor, acc.y);

					sf::Vector2f vel = data->vel[i];
					data->vel[i] = sf::Vector2f(-vel.x * bounceFactor, vel.y);
				}
			}
		}
	}


	void VerticalCollider::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		if (invert) {
			for (int i = 0; i < endId; ++i) {
				if (data->pos[i].y < pos) {
					data->pos[i].y = pos;

					sf::Vector2f acc = data->acc[i];
					data->acc[i] = sf::Vector2f(acc.x, -acc.y * bounceFactor);

					sf::Vector2f vel = data->vel[i];
					data->vel[i] = sf::Vector2f(vel.x, -vel.y * bounceFactor);
				}
			}
		}
		else {
			for (int i = 0; i < endId; ++i) {
				if (data->pos[i].y > pos) {
					data->pos[i].y = pos;

					sf::Vector2f acc = data->acc[i];
					data->acc[i] = sf::Vector2f(acc.x, -acc.y * bounceFactor);

					sf::Vector2f vel = data->vel[i];
					data->vel[i] = sf::Vector2f(vel.x, -vel.y * bounceFactor);
				}
			}
		}
	}


	void AttractorUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;
		const int numAttractors = m_attractors.size();
		sf::Vector2f off;
		float dist;

		for (int i = 0; i < endId; ++i) {
			for (int j = 0; j < numAttractors; ++j) {
				off.x = m_attractors[j].x - data->pos[i].x;
				off.y = m_attractors[j].y - data->pos[i].y;
				dist = dot(off, off);
				dist = m_attractors[j].z / dist;

				data->acc[i] += off * dist;
			}
		}
	}


	void SizeUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			data->size[i].x = lerpFloat(data->size[i].y, data->size[i].z, a);
		}
	}


	void ColorUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			data->col[i] = lerpColor(data->startCol[i], data->endCol[i], a);
		}
	}


	void TimeUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		for (int i = 0; i < endId; ++i) {
			data->time[i].x -= dt;
			data->time[i].z = 1.0f - (data->time[i].x / data->time[i].y);

			if (data->time[i].x < 0.0f) {
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
