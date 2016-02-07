#include "Level/DynamicTiles/SimulatedWaterTile.h"
#include "Spell.h"

#include "Level/DynamicTiles/FrozenWaterTile.h"
#include "CustomParticleUpdaters.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Water, SimulatedWaterTile)

using namespace std;

const float SimulatedWaterTile::WATER_SURFACE_THICKNESS = 4.f;
const int SimulatedWaterTile::NUMBER_COLUMNS_PER_SUBTILE = 10;

const int SimulatedWaterTile::SIN_COUNT = 10;
const float SimulatedWaterTile::SIN_SPEED = 3.f;
const float SimulatedWaterTile::SIN_MAX_HEIGHT = 1.f;
const float SimulatedWaterTile::SIN_MAX_FREQUENCY = 0.1f;

inline float randomFloat(float low, float high) {
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

SimulatedWaterTile::SimulatedWaterTile(Level* level) : LevelDynamicTile(level) {
	m_time = 0.f;

	for (int i = 0; i < SIN_COUNT; ++i) {
		m_sinOffsets.push_back(randomFloat(-1.f, 1.f));
		m_sinAmplitudes.push_back(randomFloat(0.f, SIN_MAX_HEIGHT));
		m_sinFrequencies.push_back(randomFloat(0.f, SIN_MAX_FREQUENCY));
		m_sinOffsetFrequencies.push_back(randomFloat(0.f, SIN_MAX_FREQUENCY));
	}
}

void SimulatedWaterTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void SimulatedWaterTile::loadAnimation(int skinNr) {
	m_data = SimulatedWaterTileData::getData(skinNr);

	const sf::FloatRect *bb = getBoundingBox();
	m_x = getPosition().x;
	m_y = getPosition().y;
	m_width = bb->width;
	m_height = bb->height;

	setBoundingBox(sf::FloatRect(m_x, m_y + 11.f, m_width, m_height));

	m_nTiles = static_cast<int>(bb->width / TILE_SIZE);

	m_nColumns = NUMBER_COLUMNS_PER_SUBTILE * m_nTiles;
	m_columns = vector<WaterColumn>();
	for (int i = 0; i < m_nColumns; ++i) {
		WaterColumn c;
		c.targetHeight = m_height - 10;
		c.height = m_height - 10;
		c.velocity = 0.f;
		c.fixed = false;

		m_columns.push_back(c);
	}

	m_leftDeltas = new float[m_nColumns];
	m_rightDeltas = new float[m_nColumns];

	m_frozenTiles = std::vector<FrozenWaterTile *>(m_nTiles, nullptr);

	m_vertexArray = sf::VertexArray(sf::Quads, 2 * 4 * (m_nColumns - 1));

	// Particle System
	m_ps = std::unique_ptr<particles::MetaballParticleSystem>(new particles::MetaballParticleSystem(100, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob), WINDOW_WIDTH, WINDOW_HEIGHT));
	g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)->setSmooth(true);
	m_ps->color = m_data.color;
	m_ps->threshold = 0.7f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::DiskPositionGenerator>();
	m_particlePosition = &posGen->center;
	posGen->radius = 40.f;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 8.f;
	sizeGen->maxStartSize = 16.0f;
	sizeGen->minEndSize = 4.0f;
	sizeGen->maxEndSize = 8.0f;

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -60.f;
	velGen->maxAngle = 60.f;
	m_particleMinSpeed = &velGen->minStartVel;
	m_particleMaxSpeed = &velGen->maxStartVel;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 10.0f;
	timeGen->maxTime = 10.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();

	auto waterUpdater = m_ps->addUpdater<particles::SimulatedWaterUpdater>();
	waterUpdater->water = this;

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();
	eulerUpdater->globalAcceleration = sf::Vector2f(0.0f, 500.0f);
}

void SimulatedWaterTile::update(const sf::Time& frameTime) {
	float dt = frameTime.asSeconds();
	dt *= 20.f;

	m_time += SIN_SPEED * dt;

	for (int i = 0; i < m_nColumns; ++i) {
		m_columns[i].update(m_data.damping, m_data.tension, dt);
	}

	for (int iterations = 0; iterations < 8; ++iterations) {
		for (int i = 0; i < m_nColumns; ++i) {
			if (i > 0) {
				m_leftDeltas[i] = m_data.spread * (m_columns[i].height - m_columns[i - 1].height);
				m_columns[i - 1].velocity += m_leftDeltas[i] * dt;
			}
			if (i < m_nColumns - 1) {
				m_rightDeltas[i] = m_data.spread * (m_columns[i].height - m_columns[i + 1].height);
				m_columns[i + 1].velocity += m_rightDeltas[i] * dt;
			}
		}

		for (int i = 0; i < m_nColumns; ++i) {
			if (i > 0) {
				m_columns[i - 1].height += m_leftDeltas[i] * dt;
			}
			if (i < m_nColumns - 1) {
				m_columns[i + 1].height += m_rightDeltas[i] * dt;
			}
		}
	}

	if (m_columns[0].height > m_height) {
		m_columns[0].height = m_height;
	}
	if (m_columns[m_nColumns - 1].height > m_height) {
		m_columns[m_nColumns - 1].height = m_height;
	}

	for (int i = 0; i < m_nColumns; ++i) {
		if (m_columns[i].fixed) {
			m_columns[i].height = m_columns[i].targetHeight;
		}
	}

	sf::Color transparent = sf::Color(255, 255, 255, 0);

	float scale = m_width / (float)(m_nColumns - 1);
	float thickness = 4.f;

	for (int i = 0; i < m_nColumns - 1; ++i) {
		bool fixed1 = m_columns[i].fixed;
		bool fixed2 = m_columns[i+1].fixed;

		float heightOffset1 = fixed1 ? 0.f : getSinHeight(i * scale, m_time);
		float heightOffset2 = fixed2 ? 0.f : getSinHeight((i + 1) * scale, m_time);

		sf::Vector2f p1 = sf::Vector2f(m_x + i * scale, m_y + m_height - m_columns[i].height - heightOffset1);
		sf::Vector2f p2 = sf::Vector2f(m_x + (i + 1) * scale, m_y + m_height - m_columns[i + 1].height - heightOffset2);
		sf::Vector2f p3 = sf::Vector2f(p2.x, m_y + m_height);
		sf::Vector2f p4 = sf::Vector2f(p1.x, m_y + m_height);
		sf::Vector2f p5 = sf::Vector2f(p2.x, p2.y - thickness);
		sf::Vector2f p6 = sf::Vector2f(p1.x, p1.y - thickness);

		m_vertexArray[8 * i + 0].position = p1;
		m_vertexArray[8 * i + 0].color = m_data.color;
		m_vertexArray[8 * i + 1].position = p2;
		m_vertexArray[8 * i + 1].color = m_data.color;
		m_vertexArray[8 * i + 2].position = p3;
		m_vertexArray[8 * i + 2].color = m_data.color;
		m_vertexArray[8 * i + 3].position = p4;
		m_vertexArray[8 * i + 3].color = m_data.color;

		m_vertexArray[8 * i + 4].position = p1;
		m_vertexArray[8 * i + 4].color = m_data.color;
		m_vertexArray[8 * i + 5].position = p2;
		m_vertexArray[8 * i + 5].color = m_data.color;
		m_vertexArray[8 * i + 6].position = p5;
		m_vertexArray[8 * i + 6].color = transparent;
		m_vertexArray[8 * i + 7].position = p6;
		m_vertexArray[8 * i + 7].color = transparent;
	}

	m_ps->update(frameTime);
}

float SimulatedWaterTile::getHeight(float xPosition) const {
	int index = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	if (index < 0 || index > m_nColumns - 1)
		return m_height - 10;

	return m_columns[index].height;
}

float SimulatedWaterTile::getSinHeight(float x, float t) const {
	float result = 0.f;
	for (int i = 0; i < SIN_COUNT; ++i) {
		result += m_sinOffsets[i] + m_sinAmplitudes[i] * std::sin(x * m_sinFrequencies[i] + t * m_sinOffsetFrequencies[i]);
	}
	return result;
}

void SimulatedWaterTile::splash(float xPosition, float velocity) {
	int index = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	if (index > 0 && index < m_nColumns) {
		m_columns[index].velocity = velocity;
	}

	velocity = std::abs(velocity);
	float y = getHeight(xPosition);
	const sf::FloatRect *bb = getBoundingBox();

	*m_particlePosition = sf::Vector2f(xPosition, bb->top + bb->height - y - WATER_SURFACE_THICKNESS);
	*m_particleMinSpeed = 0.2f * velocity;
	*m_particleMaxSpeed = 1.0f * velocity;
	int nParticles = static_cast<int>(velocity / 8);
	m_ps->emit(nParticles);
	if (velocity > 100.f)
		g_resourceManager->playSound(m_sound, m_data.sound);
}

void SimulatedWaterTile::splash(float xPosition, float width, float velocity) {
	int startIndex = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	int endIndex = static_cast<int>((xPosition + width - m_x) / (m_width / (m_nColumns - 1)));
	for (int i = startIndex; i <= endIndex; ++i) {
		if (i > 0 && i < m_nColumns) {
			m_columns[i].velocity = 0.1f * velocity;
		}
	}

	velocity = std::abs(velocity);
	xPosition = xPosition + 0.5f * width;
	float y = getHeight(xPosition);
	const sf::FloatRect *bb = getBoundingBox();

	*m_particlePosition = sf::Vector2f(xPosition, bb->top + bb->height - y - WATER_SURFACE_THICKNESS);
	*m_particleMinSpeed = 0.2f * velocity;
	*m_particleMaxSpeed = 1.0f * velocity;
	int nParticles = static_cast<int>(velocity / 8);
	m_ps->emit(nParticles);
	if (velocity > 100.f)
		g_resourceManager->playSound(m_sound, m_data.sound);
}

void SimulatedWaterTile::render(sf::RenderTarget& target) {
	target.draw(m_vertexArray);
	m_ps->render(target);
}

void SimulatedWaterTile::onHit(Spell* spell) {
	auto id = spell->getSpellID();

	int index = static_cast<int>(std::floor((spell->getPosition().x - m_x) / TILE_SIZE));
	bool frozen = isFrozen(index);
	bool doSplash = !frozen;

	if (id == SpellID::IceBall) {
		if (!frozen && m_data.isFreezable) {
			freeze(index);
			spell->setDisposed();
			doSplash = false;
		}
	}

	if (doSplash) {
		float vx = spell->getVelocity().x;
		float vy = spell->getVelocity().y;
		float vel = std::sqrt(vx*vx + vy*vy);
		splash(spell->getPosition().x, -vel * 0.5f);
		spell->setDisposed();
	}
}

void SimulatedWaterTile::onHit(LevelMovableGameObject* mob) {
	// don't splash if the mob is deeper than one tile below the surface
	if (mob->getBoundingBox()->top > getBoundingBox()->top + TILE_SIZE) return;
	float vx = mob->getVelocity().x;
	float vy = mob->getVelocity().y;
	float vel = std::sqrt(vx*vx + vy*vy);
	// TODO: find maximum value for velocity, s.t. the waves stay inside the tile
	splash(mob->getBoundingBox()->left, mob->getBoundingBox()->width, -vel * 0.5f);
	if (m_data.isDeadly) {
		mob->setDead();
	}
}

void SimulatedWaterTile::freeze(int index) {
	if (index >= 0 && index < m_nTiles) {
		// check if this water tile can be frozen or if a mob is in the way
		WorldCollisionQueryRecord rec;
		rec.boundingBox = sf::FloatRect(m_x + index * TILE_SIZE, m_y, static_cast<float>(TILE_SIZE), TILE_SIZE_F);
		if (m_level->collidesWithMobs(rec)) {
			g_logger->logInfo("SimulatedWaterTile::freeze", "Cannot freeze this tile as it would stuck a MOB!");
			return;
		}

		for (int i = 0; i < NUMBER_COLUMNS_PER_SUBTILE; ++i) {
			WaterColumn &col = m_columns[index * NUMBER_COLUMNS_PER_SUBTILE + i];
			col.height = col.targetHeight;
			col.fixed = true;
		}

		FrozenWaterTile *frozenTile = new FrozenWaterTile(this, index);
		frozenTile->init();
		frozenTile->setPosition(sf::Vector2f(m_x + index * TILE_SIZE, m_y));
		const sf::FloatRect *bb = frozenTile->getBoundingBox();
		frozenTile->setBoundingBox(sf::FloatRect(bb->left, bb->top, bb->width, bb->height - 35.f));	// ice tile is ca. 15 pixels thick
		frozenTile->setDebugBoundingBox(sf::Color::Yellow);
		frozenTile->loadAnimation(0);
		m_frozenTiles[index] = frozenTile;
		m_screen->addObject(frozenTile);
	}
}

void SimulatedWaterTile::melt(int index) {
	if (index >= 0 && index < m_nTiles) {
		for (int i = 0; i < NUMBER_COLUMNS_PER_SUBTILE; ++i) {
			WaterColumn &col = m_columns[index * NUMBER_COLUMNS_PER_SUBTILE + i];
			col.fixed = false;
		}

		m_frozenTiles[index] = nullptr;	// LET IT GOOOO, LET IT GO. Can't keep this pointer anymoooreee.
	}
}

bool SimulatedWaterTile::isFrozen(int index) {
	if (m_frozenTiles.size() == 0) return false;
	if (index < 0) index = 0;
	if (index >= m_frozenTiles.size()) index = static_cast<int>(m_frozenTiles.size()) - 1;
	return (m_frozenTiles[index] != nullptr);
}