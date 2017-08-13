#pragma once

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleHelpers.h"
#include "Enums/DamageType.h"

class FluidTile;
class LevelMovableGameObject;
class Level;

namespace particles
{
	class FluidUpdater : public ParticleUpdater {
	public:
		FluidUpdater() {}
		~FluidUpdater() {}

		void update(ParticleData* data, float dt);

	public:
		FluidTile* fluidTile = nullptr;
	};

	class FadingColorUpdater final : public ColorUpdater {
	public:
		FadingColorUpdater() {}

		void update(ParticleData* data, float dt) override;

		// start the fading timer (in seconds)
		void setFading(float timeToFade);
		void resetColor();

	private:
		float m_timeToFade;
		float m_initialTimeToFade;
		bool m_isFading = false;
	};


	class AttractingEulerUpdater : public EulerUpdater {
	public:
		AttractingEulerUpdater(const sf::Vector2f* refPos, float fraction = 1.f);
		~AttractingEulerUpdater() {}

		void update(ParticleData *data, float dt) override;

	public:
		const sf::Vector2f* m_refPos;
		sf::Vector2f m_oldPos;
		float m_fraction;
	};

	class DamagingUpdater : public ParticleUpdater {
	public:
		DamagingUpdater(LevelMovableGameObject* mob, int damage, DamageType damageType);
		~DamagingUpdater() {}

		void update(ParticleData *data, float dt) override;

	public:
		LevelMovableGameObject* m_mob;
		int m_damage;
		DamageType m_damageType;
	};

	class CollidingUpdater : public ParticleUpdater {
	public:
		CollidingUpdater(const Level* level);
		~CollidingUpdater() {}

		void update(ParticleData *data, float dt) override;

	public:
		const Level* m_level;
	};

	class EllipseSpawner : public ParticleSpawner {
	public:
		EllipseSpawner() {}
		~EllipseSpawner() {}

		void spawn(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f radius;
	};
}
