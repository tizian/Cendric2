#pragma once

#include "ParticleData.h"
#include "ParticleUpdater.h"
#include "ParticleGenerator.h"

namespace particles
{
	class ParticleSystem : public sf::Drawable, public sf::Transformable
	{
	public:
		explicit ParticleSystem(size_t maxCount, sf::Texture *tex=nullptr);
		virtual ~ParticleSystem() {}

		ParticleSystem(const ParticleSystem &) = delete;
		ParticleSystem &operator=(const ParticleSystem &) = delete;

		virtual void update(const sf::Time &dt);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void reset();

		virtual size_t numAllParticles() const { return m_particles.count; }
		virtual size_t numAliveParticles() const { return m_particles.countAlive; }

		template<typename T>
		std::shared_ptr<T> addGenerator()
		{
			auto g = std::make_shared<T>();
			m_generators.push_back(g);
			return g;
		}

		template<typename T>
		std::shared_ptr<T> addUpdater()
		{
			auto u = std::make_shared<T>();
			m_updaters.push_back(u);
			return u;
		}

		void emit(int maxCount);	// emit a fix number of particles
		void emit(float dt);		// emit a stream of particles defined by emitRate and dt

	public:
		float	emitRate{ 0.0f };		// Note: For a constant particle stream, it should hold that: emitRate <= (maximalParticleCount / averageParticleLifetime)
		bool	active{ true };
		bool	additiveBlendMode{ false };

	protected:
		sf::Texture *m_texture;

		ParticleData m_particles;
		sf::VertexArray m_vertices;

		size_t m_count;

		std::vector<std::shared_ptr<ParticleGenerator>> m_generators;
		std::vector<std::shared_ptr<ParticleUpdater>> m_updaters;
	};
}
