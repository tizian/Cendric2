#include "Map/Map.h"

Map::Map() {
	m_worldData = &m_mapData;
}

Map::~Map() {
}

bool Map::load(const std::string& id) {
	MapReader reader;
	if (!reader.readMap(id.c_str(), m_mapData)) {
		return false;
	}

	m_mapData.id = id;

	// load map
	m_backgroundTileMap.load(m_mapData, m_mapData.backgroundTileLayers);
	m_lightedForegroundTileMap.load(m_mapData, m_mapData.lightedForegroundTileLayers);
	m_foregroundTileMap.load(m_mapData, m_mapData.foregroundTileLayers);
	g_resourceManager->loadMapResources();
	return true;
}

void Map::dispose() {
	World::dispose();
	g_resourceManager->deleteMapResources();
}

void Map::loadAfterMainChar(Screen* screen) {
	MapLoader loader;
	loader.loadNpcs(m_mapData, screen, this);
}

void Map::loadForRenderTexture(Screen* screen) {
	MapLoader loader;
	loader.loadLights(m_mapData, screen);
	loader.loadDynamicTiles(m_mapData, screen, this);
}

void Map::setWorldView(sf::RenderTarget &target, const sf::Vector2f& center) const {
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_mapData.mapRect.width - WINDOW_WIDTH / 2.f, center.x));
	float camCenterY = std::max((WINDOW_HEIGHT) / 2.f, std::min(m_mapData.mapRect.height - (WINDOW_HEIGHT) / 2.f, center.y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
}

bool Map::collides(WorldCollisionQueryRecord& rec) const {
	// additional : check for collision with map rect (y axis)
	if (rec.boundingBox.top < m_mapData.mapRect.top || rec.boundingBox.top + rec.boundingBox.height > m_mapData.mapRect.top + m_mapData.mapRect.height) {
		if (rec.collisionDirection == CollisionDirection::Down) {
			rec.safeTop = m_worldData->mapRect.top + m_worldData->mapRect.height - rec.boundingBox.height;
		}
		if (rec.collisionDirection == CollisionDirection::Up) {
			rec.safeTop = m_worldData->mapRect.top;
		}
		return true;
	}
	return World::collides(rec);
}

MapExitData* Map::checkLevelEntry(const sf::FloatRect& boundingBox) const {
	for (auto it : m_mapData.mapExits) {
		if (boundingBox.intersects(it.mapExitRect)) {
			MapExitData* exit = new MapExitData(it);
			return exit;
		}
	}

	return nullptr;
}
