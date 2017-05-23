#include "Level/DynamicTiles/FluidTile.h"
#include "Spells/Spell.h"
#include "World/MovableGameObject.h"
#include "Level/LevelMainCharacter.h"

#include "Level/DynamicTiles/FrozenWaterTile.h"
#include "CustomParticleUpdaters.h"
#include "GlobalResource.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Fluid, FluidTile)

const float FluidTile::SURFACE_THICKNESS = 4.f;
const int FluidTile::NUMBER_COLUMNS_PER_SUBTILE = 10;

FluidTile::FluidTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_isRenderAfterObjects = true;
}

FluidTile::~FluidTile() {
	for (auto const& entry : m_soundMap) {
		delete entry.second;
	}
	m_soundMap.clear();
	delete m_ps;
}

void FluidTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void FluidTile::loadAnimation(int skinNr) {
	m_data = FluidTileData::getData(skinNr);
	g_resourceManager->loadSoundbuffer(m_data.soundPath, ResourceType::Level);

	const sf::FloatRect* bb = getBoundingBox();
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

	m_frozenTiles = std::vector<FrozenWaterTile*>(m_nTiles, nullptr);

	m_vertexArray = sf::VertexArray(sf::Quads, 2 * 4 * (m_nColumns - 1));

	// Particle System
	int maxNumberParticles = m_nTiles * 50;
	m_ps = new particles::MetaballParticleSystem(maxNumberParticles, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB), WINDOW_WIDTH, WINDOW_HEIGHT);
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB)->setSmooth(true);
	m_ps->color = m_data.color;
	m_ps->threshold = 0.7f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	m_emitterPosition = &posGen->center;
	m_emitterSize = &posGen->size;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 16.f;
	sizeGen->maxStartSize = 32.0f;
	sizeGen->minEndSize = 8.0f;
	sizeGen->maxEndSize = 16.0f;

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -40.f;
	velGen->maxAngle = 40.f;
	m_particleMinSpeed = &velGen->minStartSpeed;
	m_particleMaxSpeed = &velGen->maxStartSpeed;

	// Updaters
	auto fluidUpdater = m_ps->addUpdater<particles::FluidUpdater>();
	fluidUpdater->fluidTile = this;

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();
	eulerUpdater->globalAcceleration = sf::Vector2f(0.0f, 500.0f);
}

void FluidTile::update(const sf::Time& frameTime) {
	checkForMovableTiles();

	float dt = frameTime.asSeconds();
	dt *= 8;

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

void FluidTile::splash(const MovableGameObject* source, float xPosition, float width, sf::Vector2f velocity, float waveVelocityScale, float particleVelocityScale) {
	velocity = velocity * m_data.velocityScale;	// Apply global scale factor based on fluid parameters
	float velocityNorm = norm(velocity);

	if (velocityNorm < Epsilon) return;
	
	// Adjust velocity of water surface
	int startIndex = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	int endIndex = static_cast<int>((xPosition + width - m_x) / (m_width / (m_nColumns - 1)));
	for (int i = startIndex; i <= endIndex; ++i) {
		if (i > 0 && i < m_nColumns) {
			m_columns[i].velocity = -1.f * waveVelocityScale * velocity.y;
		}
	}

	// Create particle splashes
	float particleVelocity = particleVelocityScale * std::abs(velocity.y);
	float waterHeight = getHeight(xPosition + 0.5f * width);
	const sf::FloatRect* bb = getBoundingBox();

	*m_emitterPosition = sf::Vector2f(xPosition + 0.5f * width, bb->top + bb->height - waterHeight);
	*m_emitterSize = sf::Vector2f(0.8f * width, 2.f);
	*m_particleMinSpeed = 0.2f * particleVelocity;
	*m_particleMaxSpeed = 1.0f * particleVelocity;

	int nParticles = static_cast<int>(0.05f * particleVelocity);
	m_ps->emitParticles(nParticles);

	// Play sound
	if (velocityNorm > 100.f && source) {
		float distance = dist(sf::Vector2f(source->getBoundingBox()->left, source->getBoundingBox()->top), m_mainChar->getPosition());
		if (distance > WINDOW_WIDTH) return;
		if (!contains(m_soundMap, source)) {
			m_soundMap.insert({ source, new sf::Sound() });
		}

		float scale = 1.f - distance / WINDOW_WIDTH;
		scale /= 2.f;

		g_resourceManager->playSound(*m_soundMap.at(source), m_data.soundPath, false, false, scale);
	}
}

void FluidTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		m_isFirstRenderIteration = false;
		return;
	}
	m_isFirstRenderIteration = true;
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
		splash(spell, spell->getPosition().x, 0.f, spell->getVelocity());
		spell->setDisposed();
	}
}

void FluidTile::onHit(LevelMovableGameObject* mob) {
	// don't splash if the mob is deeper than one tile below the surface
	if (mob->getBoundingBox()->top > getBoundingBox()->top + TILE_SIZE) return;

	splash(mob, mob->getBoundingBox()->left, mob->getBoundingBox()->width, mob->getVelocity(), 0.1f, 0.5f);

	if (m_data.isDeadly) {
		mob->setDead();
	}
}

void FluidTile::checkForMovableTiles() {
	for (auto& it : *m_level->getMovableTiles()) {
		MovableGameObject* tile = dynamic_cast<MovableGameObject*>(it);
		if (tile == nullptr) continue;

		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (m_boundingBox.intersects(tileBB)) {
			if (tileBB.top > getBoundingBox()->top + TILE_SIZE) continue;

			int index = static_cast<int>(std::floor((tileBB.left - m_x) / TILE_SIZE));
			if (isFrozen(index)) continue;

			splash(tile, tileBB.left, tileBB.width, tile->getVelocity(), 0.1f, 0.5f);
		}
	}
	for (auto& it : *m_level->getDynamicTiles()) {
		MovableGameObject* tile = dynamic_cast<MovableGameObject*>(it);
		if (tile == nullptr) continue;

		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (m_boundingBox.intersects(tileBB)) {
			if (tileBB.top > getBoundingBox()->top + TILE_SIZE) continue;

			int index = static_cast<int>(std::floor((tileBB.left - m_x) / TILE_SIZE));
			if (isFrozen(index)) continue;

			splash(tile, tileBB.left, tileBB.width, tile->getVelocity(), 0.1f, 0.5f);
		}
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
			FluidColumn& col = m_columns[index * NUMBER_COLUMNS_PER_SUBTILE + i];
			col.height = col.targetHeight;
			col.fixed = true;
		}

		FrozenWaterTile* frozenTile = new FrozenWaterTile(this, index);
		frozenTile->init();
		frozenTile->setPosition(sf::Vector2f(m_x + index * TILE_SIZE, m_y));
		const sf::FloatRect* bb = frozenTile->getBoundingBox();
		frozenTile->setBoundingBox(sf::FloatRect(bb->left, bb->top, bb->width, bb->height - 35.f));	// ice tile is ca. 15 pixels thick
		frozenTile->setDebugBoundingBox(COLOR_NEUTRAL);
		frozenTile->loadResources();
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