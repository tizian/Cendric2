#include "Particles/ParticleSystem.h"

namespace particles
{
	ParticleSystem::ParticleSystem(int maxCount) : m_count(maxCount), m_particles(maxCount) {
		for (int i = 0; i < maxCount; ++i) {
			m_particles.alive[i] = false;
		}
	}

	void ParticleSystem::emit(float dt) {
		m_dt += dt;

		int maxNewParticles = 0;

		if (m_dt * emitRate > 1.0f) {
			maxNewParticles = static_cast<int>(m_dt * emitRate);
			m_dt -= maxNewParticles / emitRate;
		}

		if (maxNewParticles == 0) return;

		const int startId = m_particles.countAlive;
		const int endId = std::min(startId + maxNewParticles, m_particles.count - 1);

		for (auto &generator : m_generators) {
			generator->generate(&m_particles, startId, endId);
		}

		for (int i = startId; i < endId; ++i) {
			m_particles.wake(i);
		}
	}

	void ParticleSystem::emit(int maxCount) {
		const int startId = m_particles.countAlive;
		const int endId = std::min(startId + maxCount, m_particles.count - 1);

		for (auto &generator : m_generators) {
			generator->generate(&m_particles, startId, endId);
		}

		for (int i = startId; i < endId; ++i) {
			m_particles.wake(i);
		}
	}

	void ParticleSystem::update(const sf::Time &dt) {
		if (emitRate > 0.0f) {
			emit(dt.asSeconds());
		}

		for (int i = 0; i < m_particles.countAlive; ++i) {
			m_particles.acc[i] = { 0.0f, 0.0f };
		}

		for (auto & updater : m_updaters) {
			updater->update(&m_particles, dt.asSeconds());
		}
	}

	void ParticleSystem::reset() {
		m_particles.countAlive = 0;
	}



	PointParticleSystem::PointParticleSystem(int maxCount) : ParticleSystem(maxCount) {
		m_vertices = sf::VertexArray(sf::Points, maxCount);
	}

	void PointParticleSystem::update(const sf::Time &dt) {
		ParticleSystem::update(dt);

		for (int i = 0; i < m_particles.countAlive; ++i) {
			m_vertices[i].position = m_particles.pos[i];
			m_vertices[i].color = m_particles.col[i];
		}
	}

	void PointParticleSystem::render(sf::RenderTarget &renderTarget) {
		sf::RenderStates states = sf::RenderStates::Default;

		const sf::Vertex *ver = &m_vertices[0];
		renderTarget.draw(ver, m_particles.countAlive, sf::Points, states);
	}



	TextureParticleSystem::TextureParticleSystem(int maxCount, sf::Texture *texture) : ParticleSystem(maxCount), m_texture(texture) {
		m_vertices = sf::VertexArray(sf::Quads, maxCount * 4);

		for (int i = 0; i < m_particles.count; ++i) {
			m_vertices[4 * i + 0].texCoords.x = 0.0f;	m_vertices[4 * i + 0].texCoords.y = 0.0f;
			m_vertices[4 * i + 1].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 1].texCoords.y = 0.0f;
			m_vertices[4 * i + 2].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 2].texCoords.y = (float)m_texture->getSize().y;
			m_vertices[4 * i + 3].texCoords.x = 0.0f;	m_vertices[4 * i + 3].texCoords.y = (float)m_texture->getSize().y;

			m_vertices[4 * i + 0].color = sf::Color::White;
			m_vertices[4 * i + 1].color = sf::Color::White;
			m_vertices[4 * i + 2].color = sf::Color::White;
			m_vertices[4 * i + 3].color = sf::Color::White;
		}

		additiveBlendMode = false;
	}

	void TextureParticleSystem::setTexture(sf::Texture *texture) {
		m_texture = texture;

		for (int i = 0; i < m_particles.count; ++i) {
			m_vertices[4 * i + 0].texCoords.x = 0.0f;	m_vertices[4 * i + 0].texCoords.y = 0.0f;
			m_vertices[4 * i + 1].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 1].texCoords.y = 0.0f;
			m_vertices[4 * i + 2].texCoords.x = (float)m_texture->getSize().x;	m_vertices[4 * i + 2].texCoords.y = (float)m_texture->getSize().y;
			m_vertices[4 * i + 3].texCoords.x = 0.0f;	m_vertices[4 * i + 3].texCoords.y = (float)m_texture->getSize().y;
		}
	}

	void TextureParticleSystem::update(const sf::Time &dt) {
		ParticleSystem::update(dt);

		for (int i = 0; i < m_particles.countAlive; ++i) {
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

	void TextureParticleSystem::render(sf::RenderTarget &renderTarget) {
		sf::RenderStates states = sf::RenderStates::Default;

		if (additiveBlendMode) {
			states.blendMode = sf::BlendAdd;
		}

		states.texture = m_texture;

		const sf::Vertex *ver = &m_vertices[0];
		renderTarget.draw(ver, m_particles.countAlive * 4, sf::Quads, states);
	}



	const std::string vertexShader = \
		"void main()" \
		"{" \
		"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;" \
		"    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;" \
		"    gl_FrontColor = gl_Color;" \
		"}";

	const std::string fragmentShader = \
		"uniform sampler2D texture;" \
		"uniform vec4 customColor;" \
		"uniform float threshold;" \
		"" \
		"void main()" \
		"{" \
		"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
		"    if (pixel.a > threshold) {" \
		"        gl_FragColor = customColor;" \
		"    }" \
		"    else {" \
		"        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);" \
		"    }" \
		"}";

	MetaballParticleSystem::MetaballParticleSystem(int maxCount, sf::Texture *texture, int windowWidth, int windowHeight) : TextureParticleSystem(maxCount, texture) {
		additiveBlendMode = true;
		m_shader.setParameter("texture", sf::Shader::CurrentTexture);
		m_shader.loadFromMemory(vertexShader, fragmentShader);
		m_renderTexture.create(windowWidth, windowHeight);
	}

	void MetaballParticleSystem::render(sf::RenderTarget &renderTarget) {
		sf::RenderStates states = sf::RenderStates::Default;
		states.blendMode = sf::BlendAdd;

		states.texture = m_texture;

		const sf::Vertex *ver = &m_vertices[0];

		sf::View oldView = renderTarget.getView();
		sf::View defaultView = renderTarget.getDefaultView();

		m_renderTexture.setView(oldView);
		m_renderTexture.clear(sf::Color(0, 0, 0, 0));
		m_renderTexture.draw(ver, m_particles.countAlive * 4, sf::Quads, states);
		m_renderTexture.display();
		m_sprite.setTexture(m_renderTexture.getTexture());
		m_shader.setParameter("customColor", color);
		m_shader.setParameter("threshold", threshold);
		
		renderTarget.setView(defaultView);
		renderTarget.draw(m_sprite, &m_shader);
		renderTarget.setView(oldView);
	}
}
