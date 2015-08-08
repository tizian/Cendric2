#include "Particles/ParticleSystem.h"

namespace particles
{
	ParticleSystem::ParticleSystem(size_t maxCount, sf::Texture *tex) : m_particles(maxCount)
	{
		m_count = maxCount;

		for (size_t i = 0; i < maxCount; ++i)
			m_particles.alive[i] = false;

		m_texture = tex;
		if (m_texture)
		{
			m_vertices = sf::VertexArray(sf::Quads, maxCount * 4);

			for (size_t i = 0; i < m_particles.count; ++i)
			{
				m_vertices[4 * i + 0].texCoords.x = 0.0f;	m_vertices[4 * i + 0].texCoords.y = 0.0f;
				m_vertices[4 * i + 1].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 1].texCoords.y = 0.0f;
				m_vertices[4 * i + 2].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 2].texCoords.y = (float)m_texture->getSize().y;
				m_vertices[4 * i + 3].texCoords.x = 0.0f;	m_vertices[4 * i + 3].texCoords.y = (float)m_texture->getSize().y;

				m_vertices[4 * i + 0].color = sf::Color::White;
				m_vertices[4 * i + 1].color = sf::Color::White;
				m_vertices[4 * i + 2].color = sf::Color::White;
				m_vertices[4 * i + 3].color = sf::Color::White;
			}
		}
		else
		{
			m_vertices = sf::VertexArray(sf::Points, maxCount);
		}
	}

	void ParticleSystem::emit(float dt)
	{
		const size_t maxNewParticles = static_cast<size_t>(dt * emitRate);
		const size_t startId = m_particles.countAlive;
		const size_t endId = std::min(startId + maxNewParticles, m_particles.count - 1);

		for (auto &gen : m_generators)
			gen->generate(&m_particles, startId, endId);

		for (size_t i = startId; i < endId; ++i)
		{
			m_particles.wake(i);
		}
	}

	void ParticleSystem::emit(int maxCount)
	{
		const size_t startId = m_particles.countAlive;
		const size_t endId = std::min(startId + maxCount, m_particles.count - 1);

		for (auto &gen : m_generators)
			gen->generate(&m_particles, startId, endId);

		for (size_t i = startId; i < endId; ++i)
		{
			m_particles.wake(i);
		}
	}

	void ParticleSystem::update(const sf::Time &dt)
	{
		if (active)
		{
			emit(dt.asSeconds());
		}

		for (size_t i = 0; i < m_particles.countAlive; ++i)
		{
			m_particles.acc[i] = { 0.0f, 0.0f };
		}

		for (auto & up : m_updaters)
		{
			up->update(dt.asSeconds(), &m_particles);
		}

		if (m_texture)
		{
			for (size_t i = 0; i < m_particles.countAlive; ++i)
			{
				m_vertices[4 * i + 0].position.x = m_particles.pos[i].x - m_particles.size[i].x;	m_vertices[4 * i + 0].position.y = m_particles.pos[i].y - m_particles.size[i].x;
				m_vertices[4 * i + 1].position.x = m_particles.pos[i].x + m_particles.size[i].x;	m_vertices[4 * i + 1].position.y = m_particles.pos[i].y - m_particles.size[i].x;
				m_vertices[4 * i + 2].position.x = m_particles.pos[i].x + m_particles.size[i].x;	m_vertices[4 * i + 2].position.y = m_particles.pos[i].y + m_particles.size[i].x;
				m_vertices[4 * i + 3].position.x = m_particles.pos[i].x - m_particles.size[i].x;	m_vertices[4 * i + 3].position.y = m_particles.pos[i].y + m_particles.size[i].x;

				m_vertices[4 * i + 0].color = m_particles.col[i];
				m_vertices[4 * i + 1].color = m_particles.col[i];
				m_vertices[4 * i + 2].color = m_particles.col[i];
				m_vertices[4 * i + 3].color = m_particles.col[i];
			}
		}
		else
		{
			for (size_t i = 0; i < m_particles.countAlive; ++i)
			{
				m_vertices[i].position = m_particles.pos[i];
				m_vertices[i].color = m_particles.col[i];
			}
		}

		
	}

	void ParticleSystem::reset()
	{
		m_particles.countAlive = 0;
	}

	void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if (additiveBlendMode)
		{
			states.blendMode = sf::BlendAdd;
		}
		else
		{
			states.blendMode = sf::BlendAlpha;
		}

		if (m_texture)
		{
			states.texture = m_texture;
			
			const sf::Vertex *ver = &m_vertices[0];
			target.draw(ver, m_particles.countAlive * 4, sf::Quads, states);
		}
		else
		{
			states.texture = nullptr;

			const sf::Vertex *ver = &m_vertices[0];
			target.draw(ver, m_particles.countAlive, sf::Points, states);
		}
		
	}
}
