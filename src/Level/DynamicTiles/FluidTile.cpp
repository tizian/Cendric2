#include "Level/DynamicTiles/FluidTile.h"
#include "Spells/Spell.h"

#include "Level/DynamicTiles/FrozenWaterTile.h"
#include "CustomParticleUpdaters.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Fluid, FluidTile)

const float FluidTile::SURFACE_THICKNESS = 4.f;
const int FluidTile::NUMBER_COLUMNS_PER_SUBTILE = 10;

inline float randomFloat(float low, float high) {
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

FluidTile::FluidTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_isRenderAfterObjects = true;
}

void FluidTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void FluidTile::loadAnimation(int skinNr) {
	m_data = FluidTileData::getData(skinNr);

	const sf::FloatRect *bb = getBoundingBox();
	m_x = getPosition().x;
	m_y = getPosition().y;
	m_width = bb->width;
	m_height = bb->height;

	float dHeight = 50.f - m_data.height;
	setBoundingBox(sf::FloatRect(m_x, m_y + dHeight, m_width, m_height - dHeight));

	m_nTiles = static_cast<int>(bb->width / TILE_SIZE);

	m_nColumns = NUMBER_COLUMNS_PER_SUBTILE * m_nTiles;
	m_columns = std::vector<FluidColumn>();
	for (int i = 0; i < m_nColumns; ++i) {
		FluidColumn c;
		c.targetHeight = m_height - dHeight;
		c.height = c.targetHeight;
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
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	m_particlePosition = &posGen->center;
	posGen->size = sf::Vector2f(20.f, 1.f);

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 8.f;
	sizeGen->maxStartSize = 16.0f;
	sizeGen->minEndSize = 4.0f;
	sizeGen->maxEndSize = 8.0f;

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -40.f;
	velGen->maxAngle = 40.f;
	m_particleMinSpeed = &velGen->minStartVel;
	m_particleMaxSpeed = &velGen->maxStartVel;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 10.0f;
	timeGen->maxTime = 10.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();

	auto fluidUpdater = m_ps->addUpdater<particles::FluidUpdater>();
	fluidUpdater->fluidTile = this;

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();
	eulerUpdater->globalAcceleration = sf::Vector2f(0.0f, 500.0f);
}
 
void FluidTile::checkForMovableTiles() {
	for (auto& it : *m_level->getMovableTiles()) {
		MovableGameObject* tile = dynamic_cast<MovableGameObject*>(it);
		if (tile == nullptr) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (m_boundingBox.intersects(tileBB)) {
			if (tileBB.top > getBoundingBox()->top + TILE_SIZE) continue;
			float vel = norm(tile->getVelocity());
			// TODO: find maximum value for velocity, s.t. the waves stay inside the tile
			splash(tileBB.left, tileBB.width, -vel * 0.5f);
		}
	}
	for (auto& it : *m_level->getDynamicTiles()) {
		MovableGameObject* tile = dynamic_cast<MovableGameObject*>(it);
		if (tile == nullptr) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (m_boundingBox.intersects(tileBB)) {
			if (tileBB.top > getBoundingBox()->top + TILE_SIZE) continue;
			float vel = norm(tile->getVelocity());
			// TODO: find maximum value for velocity, s.t. the waves stay inside the tile
			splash(tileBB.left, tileBB.width, -vel * 0.5f);
		}
	}
}

void FluidTile::update(const sf::Time& frameTime) {
	m_isFirstRenderIteration = true;

	checkForMovableTiles();

	float dt = frameTime.asSeconds();
	dt *= 20.f;

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

	for (int i = 0; i < m_nColumns - 1; ++i) {
		sf::Vector2f p1 = sf::Vector2f(m_x + i * scale, m_y + m_height - m_columns[i].height);
		sf::Vector2f p2 = sf::Vector2f(m_x + (i + 1) * scale, m_y + m_height - m_columns[i + 1].height);
		sf::Vector2f p3 = sf::Vector2f(p2.x, m_y + m_height);
		sf::Vector2f p4 = sf::Vector2f(p1.x, m_y + m_height);
		sf::Vector2f p5 = sf::Vector2f(p2.x, p2.y - SURFACE_THICKNESS);
		sf::Vector2f p6 = sf::Vector2f(p1.x, p1.y - SURFACE_THICKNESS);

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

float FluidTile::getHeight(float xPosition) const {
	int index = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	if (index < 0 || index > m_nColumns - 1) {
		float dHeight = 50.f - m_data.height;
		return m_height - dHeight;
	}

	return m_columns[index].height;
}

void FluidTile::splash(float xPosition, float velocity) {
	if (velocity == 0.f) return;
	int index = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	if (index > 0 && index < m_nColumns) {
		m_columns[index].velocity = velocity;
	}

	velocity = std::abs(velocity) * m_data.velocityScale;
	float y = getHeight(xPosition);
	const sf::FloatRect *bb = getBoundingBox();

	*m_particlePosition = sf::Vector2f(xPosition, bb->top + bb->height - y);
	*m_particleMinSpeed = 0.2f * velocity;
	*m_particleMaxSpeed = 1.0f * velocity;
	int nParticles = static_cast<int>(velocity / 12);
	m_ps->emit(nParticles);
	if (velocity > 100.f)
		g_resourceManager->playSound(m_sound, m_data.sound);
}

void FluidTile::splash(float xPosition, float width, float velocity) {
	if (velocity == 0.f) return;
	int startIndex = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	int endIndex = static_cast<int>((xPosition + width - m_x) / (m_width / (m_nColumns - 1)));
	for (int i = startIndex; i <= endIndex; ++i) {
		if (i > 0 && i < m_nColumns) {
			m_columns[i].velocity = 0.1f * velocity;
		}
	}

	velocity = std::abs(velocity) * m_data.velocityScale;
	xPosition = xPosition + 0.5f * width;
	float y = getHeight(xPosition);
	const sf::FloatRect *bb = getBoundingBox();

	*m_particlePosition = sf::Vector2f(xPosition, bb->top + bb->height - y);
	*m_particleMinSpeed = 0.2f * velocity;
	*m_particleMaxSpeed = 1.0f * velocity;
	int nParticles = static_cast<int>(velocity / 12);
	m_ps->emit(nParticles);
	if (velocity > 100.f)
		g_resourceManager->playSound(m_sound, m_data.sound);
}

void FluidTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		m_isFirstRenderIteration = false;
		return;
	}
	target.draw(m_vertexArray);
	m_ps->render(target);
}

void FluidTile::onHit(Spell* spell) {
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
		float vel = norm(spell->getVelocity());
		splash(spell->getPosition().x, -vel * 0.5f);
		spell->setDisposed();
	}
}

void FluidTile::onHit(LevelMovableGameObject* mob) {
	// don't splash if the mob is deeper than one tile below the surface
	if (mob->getBoundingBox()->top > getBoundingBox()->top + TILE_SIZE) return;
	float vel = norm(mob->getVelocity());
	// TODO: find maximum value for velocity, s.t. the waves stay inside the tile
	splash(mob->getBoundingBox()->left, mob->getBoundingBox()->width, -vel * 0.5f);
	if (m_data.isDeadly) {
		mob->setDead();
	}
}

void FluidTile::freeze(int index) {
	if (index >= 0 && index < m_nTiles) {
		// check if this fluid tile can be frozen or if a mob is in the way
		WorldCollisionQueryRecord rec;
		rec.boundingBox = sf::FloatRect(m_x + index * TILE_SIZE, m_y, static_cast<float>(TILE_SIZE), TILE_SIZE_F);
		if (m_level->collidesWithMobs(rec) || m_level->collidesWithMovableTiles(rec)) {
			g_logger->logInfo("FluidTile::freeze", "Cannot freeze this tile as it would stuck a MOB or a movable tile!");
			return;
		}

		for (int i = 0; i < NUMBER_COLUMNS_PER_SUBTILE; ++i) {
			FluidColumn &col = m_columns[index * NUMBER_COLUMNS_PER_SUBTILE + i];
			col.height = col.targetHeight;
			col.fixed = true;
		}

		FrozenWaterTile *frozenTile = new FrozenWaterTile(this, index);
		frozenTile->init();
		frozenTile->setPosition(sf::Vector2f(m_x + index * TILE_SIZE, m_y));
		const sf::FloatRect *bb = frozenTile->getBoundingBox();
		frozenTile->setBoundingBox(sf::FloatRect(bb->left, bb->top, bb->width, bb->height - 35.f));	// ice tile is ca. 15 pixels thick
		frozenTile->setDebugBoundingBox(COLOR_NEUTRAL);
		frozenTile->loadAnimation(0);
		m_frozenTiles[index] = frozenTile;
		m_screen->addObject(frozenTile);
	}
}

void FluidTile::melt(int index) {
	if (index >= 0 && index < m_nTiles) {
		for (int i = 0; i < NUMBER_COLUMNS_PER_SUBTILE; ++i) {
			FluidColumn& col = m_columns[index * NUMBER_COLUMNS_PER_SUBTILE + i];
			col.fixed = false;
		}

		m_frozenTiles[index] = nullptr;	// LET IT GOOOO, LET IT GO. Can't keep this pointer anymoooreee.
	}
}

bool FluidTile::isFrozen(int index) {
	if (m_frozenTiles.size() == 0) return false;
	if (index < 0) index = 0;
	if (index >= static_cast<int>(m_frozenTiles.size())) index = static_cast<int>(m_frozenTiles.size()) - 1;
	return (m_frozenTiles[index] != nullptr);
}