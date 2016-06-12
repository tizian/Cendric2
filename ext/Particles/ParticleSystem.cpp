#include "Particles/ParticleSystem.h"

#include "Particles/ParticleData.h"

#include <iostream>

namespace particles {

/* ParticleSystem */

ParticleSystem::ParticleSystem(int maxCount) : emitRate(0.f), m_dt(0.f) {
	m_particles = new ParticleData(maxCount);
}

ParticleSystem::~ParticleSystem() {
	delete m_particles;

	for (auto s : m_spawners) {
		delete s;
	}

	for (auto g : m_generators) {
		delete g;
	}

	for (auto u : m_updaters) {
		delete u;
	}
}

void ParticleSystem::emitWithRate(float dt) {
	m_dt += dt;

	int maxNewParticles = 0;

	if (m_dt * emitRate > 1.0f) {
		maxNewParticles = static_cast<int>(m_dt * emitRate);
		m_dt -= maxNewParticles / emitRate;
	}

	if (maxNewParticles == 0) return;

	emitParticles(maxNewParticles);
}

void ParticleSystem::emitParticles(int count) {
	if (m_spawners.size() == 0) return;

	const int startId = m_particles->countAlive;
	const int endId = std::min(startId + count, m_particles->count - 1);
	const int newParticles = endId - startId;

	const int nSpawners = static_cast<int>(m_spawners.size());
	const int spawnerCount = newParticles / nSpawners;
	const int remainder = newParticles - spawnerCount * nSpawners;
	int spawnerStartId = startId;
	for (int i = 0; i < nSpawners; ++i) {
		int numberToSpawn = (i < remainder) ? spawnerCount + 1 : spawnerCount;
		m_spawners[i]->spawn(m_particles, spawnerStartId, spawnerStartId + numberToSpawn);
		spawnerStartId += numberToSpawn;
	}

	m_spawners[0]->spawn(m_particles, startId, endId);

	for (auto &generator : m_generators) {
		generator->generate(m_particles, startId, endId);
	}

	m_particles->countAlive += newParticles;
}

void ParticleSystem::update(const sf::Time &dt) {
	if (emitRate > 0.0f) {
		emitWithRate(dt.asSeconds());
	}

	for (int i = 0; i < m_particles->countAlive; ++i) {
		m_particles->acc[i] = { 0.0f, 0.0f };
	}

	for (auto & updater : m_updaters) {
		updater->update(m_particles, dt.asSeconds());
	}
}

void ParticleSystem::reset() {
	m_particles->countAlive = 0;
}


/* PointParticleSystem */

PointParticleSystem::PointParticleSystem(int maxCount) : ParticleSystem(maxCount) {
	m_vertices = sf::VertexArray(sf::Points, maxCount);
}

void PointParticleSystem::render(sf::RenderTarget &renderTarget) {
	updateVertices();

	sf::RenderStates states = sf::RenderStates::Default;

	const sf::Vertex *ver = &m_vertices[0];
	renderTarget.draw(ver, m_particles->countAlive, sf::Points, states);
}

void PointParticleSystem::updateVertices() {
	for (int i = 0; i < m_particles->countAlive; ++i) {
		m_vertices[i].position = m_particles->pos[i];
		m_vertices[i].color = m_particles->col[i];
	}
}


/* TextureParticleSystem */

TextureParticleSystem::TextureParticleSystem(int maxCount, sf::Texture *texture) : ParticleSystem(maxCount), m_texture(texture) {
	m_vertices = sf::VertexArray(sf::Quads, maxCount * 4);

	float x = static_cast<float>(m_texture->getSize().x);
	float y = static_cast<float>(m_texture->getSize().y);

	for (int i = 0; i < m_particles->count; ++i) {
		m_vertices[4 * i + 0].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4 * i + 1].texCoords = sf::Vector2f(x, 0.f);
		m_vertices[4 * i + 2].texCoords = sf::Vector2f(x, y);
		m_vertices[4 * i + 3].texCoords = sf::Vector2f(0.f, y);

		m_vertices[4 * i + 0].color = sf::Color::White;
		m_vertices[4 * i + 1].color = sf::Color::White;
		m_vertices[4 * i + 2].color = sf::Color::White;
		m_vertices[4 * i + 3].color = sf::Color::White;
	}

	additiveBlendMode = false;
}

void TextureParticleSystem::setTexture(sf::Texture *texture) {
	m_texture = texture;

	float x = static_cast<float>(m_texture->getSize().x);
	float y = static_cast<float>(m_texture->getSize().y);

	for (int i = 0; i < m_particles->count; ++i) {
		m_vertices[4 * i + 0].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4 * i + 1].texCoords = sf::Vector2f(x, 0.f);
		m_vertices[4 * i + 2].texCoords = sf::Vector2f(x, y);
		m_vertices[4 * i + 3].texCoords = sf::Vector2f(0.f, y);
	}
}

void TextureParticleSystem::updateVertices() {
	for (int i = 0; i < m_particles->countAlive; ++i) {
		float size = 0.5f * m_particles->size[i].x;
		float angle = m_particles->angle[i].x;
		
		m_vertices[4 * i + 0].position.x = -size;	m_vertices[4 * i + 0].position.y = -size;
		m_vertices[4 * i + 1].position.x = +size;	m_vertices[4 * i + 1].position.y = -size;
		m_vertices[4 * i + 2].position.x = +size;	m_vertices[4 * i + 2].position.y = +size;
		m_vertices[4 * i + 3].position.x = -size;	m_vertices[4 * i + 3].position.y = +size;

		if (angle != 0.f) {
			float sin = std::sin(angle); float cos = std::cos(angle);

			for (int j = 0; j < 4; ++j) {
				float x = m_vertices[4 * i + j].position.x;
				float y = m_vertices[4 * i + j].position.y;

				m_vertices[4 * i + j].position.x = cos * x - sin * y;
				m_vertices[4 * i + j].position.y = sin * x + cos * y;
			}
		}

		m_vertices[4 * i + 0].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 0].position.y += m_particles->pos[i].y;
		m_vertices[4 * i + 1].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 1].position.y += m_particles->pos[i].y;
		m_vertices[4 * i + 2].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 2].position.y += m_particles->pos[i].y;
		m_vertices[4 * i + 3].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 3].position.y += m_particles->pos[i].y;

		m_vertices[4 * i + 0].color = m_particles->col[i];
		m_vertices[4 * i + 1].color = m_particles->col[i];
		m_vertices[4 * i + 2].color = m_particles->col[i];
		m_vertices[4 * i + 3].color = m_particles->col[i];
	}
}

void TextureParticleSystem::render(sf::RenderTarget &renderTarget) {
	updateVertices();

	sf::RenderStates states = sf::RenderStates::Default;

	if (additiveBlendMode) {
		states.blendMode = sf::BlendAdd;
	}

	states.texture = m_texture;

	const sf::Vertex *ver = &m_vertices[0];
	renderTarget.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
}


/* SpriteSheetParticleSystem */

void SpriteSheetParticleSystem::render(sf::RenderTarget &renderTarget) {
	updateVertices();
	
	sf::RenderStates states = sf::RenderStates::Default;

	if (additiveBlendMode) {
		states.blendMode = sf::BlendAdd;
	}

	states.texture = m_texture;

	const sf::Vertex *ver = &m_vertices[0];
	renderTarget.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
}

void SpriteSheetParticleSystem::updateVertices() {
	TextureParticleSystem::updateVertices();

	for (int i = 0; i < m_particles->countAlive; ++i) {
		float left = static_cast<float>(m_particles->texCoords[i].left);
		float top = static_cast<float>(m_particles->texCoords[i].top);
		float width = static_cast<float>(m_particles->texCoords[i].width);
		float height = static_cast<float>(m_particles->texCoords[i].height);

		m_vertices[4 * i + 0].texCoords = sf::Vector2f(left, top);
		m_vertices[4 * i + 1].texCoords = sf::Vector2f(left + width, top);
		m_vertices[4 * i + 2].texCoords = sf::Vector2f(left + width, top + height);
		m_vertices[4 * i + 3].texCoords = sf::Vector2f(left, top + height);
	}
}


/* MetaballParticleSystem */

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
	m_shader.setUniform("texture", sf::Shader::CurrentTexture);
	m_shader.loadFromMemory(vertexShader, fragmentShader);
	m_renderTexture.create(windowWidth, windowHeight);
}

void MetaballParticleSystem::render(sf::RenderTarget &renderTarget) {
	updateVertices();

	sf::RenderStates states = sf::RenderStates::Default;
	states.blendMode = sf::BlendAdd;

	states.texture = m_texture;

	const sf::Vertex *ver = &m_vertices[0];

	sf::View oldView = renderTarget.getView();
	sf::View defaultView = renderTarget.getDefaultView();

	m_renderTexture.setView(oldView);
	m_renderTexture.clear(sf::Color(0, 0, 0, 0));
	m_renderTexture.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
	m_renderTexture.display();
	m_sprite.setTexture(m_renderTexture.getTexture());
	sf::Glsl::Vec4 colorVec = sf::Glsl::Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	m_shader.setUniform("customColor", colorVec);
	m_shader.setUniform("threshold", threshold);
	
	renderTarget.setView(defaultView);
	renderTarget.draw(m_sprite, &m_shader);
	renderTarget.setView(oldView);
}

}