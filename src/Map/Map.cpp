#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Map/MapDynamicTile.h"

Map::Map() : World() {
	m_worldData = &m_mapData;
}

Map::~Map() {
}

bool Map::load(const std::string& id, WorldScreen* screen) {
	m_screen = screen;
	MapReader reader;
	m_mapData.id = id;
	if (!reader.readMap(id.c_str(), m_mapData, m_screen->getCharacterCore())) {
		return false;
	}

	// adjust weather
	if (const WeatherData* weather = m_screen->getCharacterCore()->getWeather(id)) {
		m_mapData.weather = *weather;
	}

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

void Map::loadAfterMainChar(MainCharacter* character) {
	MapLoader::loadAfterMainChar(m_mapData, dynamic_cast<MapScreen*>(m_screen));
}

void Map::loadForRenderTexture() {
	MapLoader::loadForRenderTexture(m_mapData, dynamic_cast<MapScreen*>(m_screen));
	m_dynamicTiles = m_screen->getObjects(GameObjectType::_DynamicTile);
}

void Map::setWorldView(sf::RenderTarget& target, const sf::Vector2f& center) const {
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_mapData.mapRect.width - WINDOW_WIDTH / 2.f, center.x));
	float camCenterY = std::max((WINDOW_HEIGHT) / 2.f, std::min(m_mapData.mapRect.height - (WINDOW_HEIGHT) / 2.f, center.y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
}

bool Map::collides(WorldCollisionQueryRecord& rec) const {
	World::collides(rec);
	// additional : check for collision with map rect (y axis)
	if (rec.boundingBox.top < m_mapData.mapRect.top || rec.boundingBox.top + rec.boundingBox.height > m_mapData.mapRect.top + m_mapData.mapRect.height) {
		if (rec.collisionDirection == CollisionDirection::Down) {
			rec.safeTop = std::min(rec.safeTop, m_worldData->mapRect.top + m_worldData->mapRect.height - rec.boundingBox.height);
		}
		if (rec.collisionDirection == CollisionDirection::Up) {
			rec.safeTop = std::max(rec.safeTop, m_worldData->mapRect.top);
		}
		rec.collides = true;
	}

	// check collidable rects
	for (auto& rect  : m_mapData.collidableRects) {
		if (epsIntersect(rect, rec.boundingBox)) {
			calculateCollisionLocations(rec, rect);
		}
	}

	// and triangles
	for (auto& triangle : m_mapData.collidableTriangles) {
		if (triangle.intersects(rec.boundingBox)) {
			rec.collides = true;
			rec.noSafePos = true;
		}
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles) {
		MapDynamicTile* tile = dynamic_cast<MapDynamicTile*>(go);

		if (rec.ignoreDynamicTiles) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile != rec.excludedGameObject && tile->isCollidable() && epsIntersect(tileBB, rec.boundingBox)) {
			calculateCollisionLocations(rec, tileBB);
		}
	}
	
	return rec.collides;
}

