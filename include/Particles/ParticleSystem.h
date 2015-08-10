#pragma once

#include "ParticleData.h"
#include "ParticleUpdater.h"
#include "ParticleGenerator.h"

namespace particles
{
	class ParticleSystem : public sf::Transformable
	{
	public:
		ParticleSystem(size_t maxCount);
		virtual ~ParticleSystem() {}

		ParticleSystem(const ParticleSystem &) = delete;
		ParticleSystem &operator=(const ParticleSystem &) = delete;

		virtual void update(const sf::Time &dt);
		virtual void render(sf::RenderTarget& renderTarget) = 0;

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

	protected:
		size_t m_count;

		ParticleData m_particles;
		sf::VertexArray m_vertices;

		std::vector<std::shared_ptr<ParticleGenerator>> m_generators;
		std::vector<std::shared_ptr<ParticleUpdater>> m_updaters;
	};

	class PointParticleSystem : public ParticleSystem
	{
	public:
		PointParticleSystem(size_t maxCount);
		virtual ~PointParticleSystem() {}

		PointParticleSystem(const PointParticleSystem &) = delete;
		PointParticleSystem &operator=(const PointParticleSystem &) = delete;

		virtual void update(const sf::Time &dt) override;
		virtual void render(sf::RenderTarget& renderTarget) override;
	};

	class TextureParticleSystem : public ParticleSystem
	{
	public:
		TextureParticleSystem(size_t maxCount, sf::Texture *texture);
		virtual ~TextureParticleSystem() {}

		TextureParticleSystem(const TextureParticleSystem &) = delete;
		TextureParticleSystem &operator=(const TextureParticleSystem &) = delete;

		virtual void update(const sf::Time &dt) override;
		virtual void render(sf::RenderTarget& renderTarget) override;

		void setAdditiveBlendMode(bool blendMode) { m_additiveBlendMode = blendMode; }

	protected:
		sf::Texture *m_texture;
		bool m_additiveBlendMode;
	};

	class MetaballParticleSystem : public TextureParticleSystem
	{
	public:
		MetaballParticleSystem(size_t maxCount, sf::Texture *texture, sf::RenderTexture *renderTexture);
		virtual ~MetaballParticleSystem() {}

		MetaballParticleSystem(const MetaballParticleSystem &) = delete;
		MetaballParticleSystem &operator=(const MetaballParticleSystem &) = delete;

		virtual void render(sf::RenderTarget& renderTarget) override;

	public:
		sf::Color color{ sf::Color::White };
		float threshold{ 0.5f };

	protected:
		sf::RenderTexture *m_renderTexture;
		sf::Sprite m_sprite;
		sf::Shader m_shader;
	};
}
