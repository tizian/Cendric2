#include "World.h"

World::World() {
}

World::~World() {
	dispose();
}

void World::dispose() {
	m_foregroundTileMap.dispose();
	m_backgroundTileMap.dispose();
	m_lightedForegroundTileMap.dispose();
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
	rec.collides = false;
	rec.safeLeft = rec.collisionDirection == CollisionDirection::Left ? 0.f : m_worldData->mapRect.width;
	rec.safeTop = rec.collisionDirection == CollisionDirection::Up ? 0.f : m_worldData->mapRect.height;
	rec.movingParent = nullptr;
	const sf::FloatRect& bb = rec.boundingBox;
	// check for collision with map rect (x axis. y axis is not checked in levels, only in maps)
	if (bb.left < m_worldData->mapRect.left || bb.left + bb.width > m_worldData->mapRect.left + m_worldData->mapRect.width) {
		if (rec.collisionDirection == CollisionDirection::Right) {
			rec.safeLeft = std::min(rec.safeLeft, m_worldData->mapRect.left + m_worldData->mapRect.width - bb.width);
		} 
		if (rec.collisionDirection == CollisionDirection::Left) {
			rec.safeLeft = std::max(rec.safeLeft, m_worldData->mapRect.left);
		}
		rec.collides = true;
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
			if (y >= static_cast<int>(m_worldData->collidableTilePositions.size()) || y < 0 || x < 0 || x >= static_cast<int>(m_worldData->collidableTilePositions[y].size())) {
				// check for out of range (happens seldom because of rounding problems above)
				continue;
			}
			if (m_worldData->collidableTilePositions[y][x]) {
				if (rec.collisionDirection == CollisionDirection::Right) {
					rec.safeLeft = std::min(rec.safeLeft, x * TILE_SIZE_F - bb.width);
				}
				if (rec.collisionDirection == CollisionDirection::Left) {
					rec.safeLeft = std::max(rec.safeLeft, (x + 1) * TILE_SIZE_F);
				}
				if (rec.collisionDirection == CollisionDirection::Up) {
					rec.safeTop = std::max(rec.safeTop, (y + 1) * TILE_SIZE_F);
				}
				if (rec.collisionDirection == CollisionDirection::Down) {
					rec.safeTop = std::min(rec.safeTop, y * TILE_SIZE_F - bb.height);
				}
				rec.collides = true;
				break; // one collision with the world rec is sufficient? No? Do we have a really fat boss here? if yes, remove the break.
			}
		}
	}

	return rec.collides;
}

void World::calculateCollisionLocations(WorldCollisionQueryRecord& rec, const sf::FloatRect& bb) const {
	if (rec.collisionDirection == CollisionDirection::Right) {
		rec.safeLeft = std::min(rec.safeLeft, bb.left - rec.boundingBox.width);
	}
	if (rec.collisionDirection == CollisionDirection::Left) {
		rec.safeLeft = std::max(rec.safeLeft, bb.left + bb.width);
	}
	if (rec.collisionDirection == CollisionDirection::Up) {
		rec.safeTop = std::max(rec.safeTop, bb.top + bb.height);
	}
	if (rec.collisionDirection == CollisionDirection::Down) {
		rec.safeTop = std::min(rec.safeTop, bb.top - rec.boundingBox.height);
	}
	rec.collides = true;
}

const std::string& World::getID() const {
	return m_worldData->id;
}

float World::getDimming() const {
	return m_worldData->weather.dimming;
}

const std::string& World::getMusicPath() const {
	return m_worldData->musicPath;
}

const std::string& World::getName() const {
	return m_worldData->name;
}