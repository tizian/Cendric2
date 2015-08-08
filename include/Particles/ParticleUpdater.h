#pragma once

#include "ParticleData.h"

namespace particles
{
	class ParticleUpdater
	{
	public:
		ParticleUpdater() { }
		virtual ~ParticleUpdater() { }

		virtual void update(float dt, ParticleData *p) = 0;
	};


	class EulerUpdater : public ParticleUpdater
	{
	public:
		EulerUpdater() {}
		~EulerUpdater() {}

		void update(float dt, ParticleData *p);

	public:
		sf::Vector2f globalAcceleration{ 0.0f, 0.0f };
	};


	class FloorUpdater : public ParticleUpdater
	{
	public:
		FloorUpdater() {}
		~FloorUpdater() {}

		void update(float dt, ParticleData *p);

	public:
		float floorY{ 0.0f };
		float bounceFactor{ 0.5f };
	};


	class AttractorUpdater : public ParticleUpdater
	{
	public:
		AttractorUpdater() {}
		~AttractorUpdater() {}

		void update(float dt, ParticleData *p);
		size_t numAttractors() const { return m_attractors.size(); }
		void add(const sf::Vector3f &attr) { m_attractors.push_back(attr); }
		sf::Vector3f &get(size_t id) { return m_attractors[id]; }

	protected:
		std::vector<sf::Vector3f> m_attractors;	// .xy is position, .z is force
	};


	class SizeUpdater : public ParticleUpdater
	{
	public:
		SizeUpdater() {}
		~SizeUpdater() {}

		void update(float dt, ParticleData *p);
	};


	class ColorUpdater : public ParticleUpdater
	{
	public:
		ColorUpdater() {}
		~ColorUpdater() {}

		void update(float dt, ParticleData *p);
	};


	class TimeUpdater : public ParticleUpdater
	{
	public:
		TimeUpdater() {}
		~TimeUpdater() {}

		void update(float dt, ParticleData *p);
	};
}
