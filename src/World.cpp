#include "World.h"

World::World() {
}

World::~World() {
	dispose();
}

void World::dispose() {
	// no need to dispose each tile map (lfg, fg + bg), as they share their resources
	m_backgroundTileMap.dispose();
}

void World::update(const sf::Time& frameTime) {
	m_backgroundTileMap.update(frameTime);
	m_foregroundTileMap.update(frameTime);
	m_lightedForegroundTileMap.update(frameTime);
}

void World::drawBackground(sf::RenderTarget& target, const sf::RenderStates& states) const {
	m_backgroundTileMap.draw(target, states);
}

void World::drawLightedForeground(sf::RenderTarget& target, const sf::RenderStates& states) const {
	m_lightedForegroundTileMap.draw(target, states);
}

void World::drawForeground(sf::RenderTarget& target, const sf::RenderStates& states) const {
	m_foregroundTileMap.draw(target, states);
}

const TileMap* World::getBackgroundTileMap() const {
	return &m_backgroundTileMap;
}

const TileMap* World::getLightedForegroundTileMap() const {
	return &m_lightedForegroundTileMap;
}

const TileMap* World::getForegroundTileMap() const {
	return &m_foregroundTileMap;
}

const sf::FloatRect& World::getWorldRect() const {
	return m_worldData->mapRect;
}

bool World::collides(WorldCollisionQueryRecord& rec) const {
	const sf::FloatRect& bb = rec.boundingBox;
	// check for collision with map rect (x axis. y axis is not checked in levels, only in maps)
	if (bb.left < m_worldData->mapRect.left || bb.left + bb.width > m_worldData->mapRect.left + m_worldData->mapRect.width) {
		return true;
	}

	// normalize bounding box values so they match our collision grid. Wondering about the next two lines? Me too. We just don't want to floor values that are exactly on the boundaries. But only those that are down and right.
	int bottomY = static_cast<int>(floor((bb.top + bb.height) / TILE_SIZE_F) == (bb.top + bb.height) / TILE_SIZE_F ? (bb.top + bb.height) / TILE_SIZE_F - 1 : floor((bb.top + bb.height) / TILE_SIZE_F));
	int rightX = static_cast<int>(floor((bb.left + bb.width) / TILE_SIZE_F) == (bb.left + bb.width) / TILE_SIZE_F ? (bb.left + bb.width) / TILE_SIZE_F - 1 : floor((bb.left + bb.width) / TILE_SIZE_F));
	sf::Vector2i topLeft(static_cast<int>(floor(bb.left / TILE_SIZE_F)), static_cast<int>(floor(bb.top / TILE_SIZE_F)));
	sf::Vector2i topRight(rightX, static_cast<int>(floor(bb.top / TILE_SIZE_F)));
	sf::Vector2i bottomLeft(static_cast<int>(floor(bb.left / TILE_SIZE_F)), bottomY);
	sf::Vector2i bottomRight(rightX, bottomY);

	// check level grid
	for (int x = topLeft.x; x <= topRight.x; x++) {
		for (int y = topLeft.y; y <= bottomLeft.y; y++) {
			if (y >= m_worldData->collidableTilePositions.size() || y < 0 || x < 0 || x >= m_worldData->collidableTilePositions[y].size()) {
				// check for out of range (happens seldom because of rounding problems above)
				continue;
			}
			if (m_worldData->collidableTilePositions[y][x]) {
				return true;
			}
		}
	}

	return false;
}

float World::getNonCollidingTop(const WorldCollisionQueryRecord& rec) const {
	WorldCollisionQueryRecord rec2 = rec;
	sf::FloatRect& bb = rec2.boundingBox;
	bb.top = std::floor(bb.top);

	while (bb.top > 0.f && collides(rec2)) {
		bb.top -= 1.f;
	}
	return bb.top;
}

float World::getNonCollidingBottom(const WorldCollisionQueryRecord& rec) const {
	WorldCollisionQueryRecord rec2 = rec;
	sf::FloatRect& bb = rec2.boundingBox;
	bb.top = std::ceil(bb.top);

	while (bb.top + bb.height < m_worldData->mapRect.height && collides(rec2)) {
		bb.top += 1.f;
	}
	return bb.top;
}

float World::getNonCollidingLeft(const WorldCollisionQueryRecord& rec) const {
	WorldCollisionQueryRecord rec2 = rec;
	sf::FloatRect& bb = rec2.boundingBox;
	bb.left = std::floor(bb.left);

	while (bb.left > 0.f && collides(rec2)) {
		bb.left -= 1.f;
	}
	return bb.left;
}

float World::getNonCollidingRight(const WorldCollisionQueryRecord& rec) const {
	WorldCollisionQueryRecord rec2 = rec;
	sf::FloatRect& bb = rec2.boundingBox;
	bb.left = std::ceil(bb.left);

	while (bb.left + bb.width < m_worldData->mapRect.width && collides(rec2)) {
		bb.left += 1.f;
	}
	return bb.left;
}

const std::string& World::getID() const {
	return m_worldData->id;
}

float World::getDimming() const {
	return m_worldData->dimming;
}

const std::string& World::getMusicPath() const {
	return m_worldData->musicPath;
}

const std::string& World::getName() const {
	return m_worldData->name;
}