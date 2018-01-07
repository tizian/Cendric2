#include "Level/Level.h"
#include "Screens/LevelScreen.h"
#include "Level/DynamicTiles/MovingTile.h"

const float Level::CAMERA_WINDOW_HEIGHT = 200.f;
const float Level::CAMERA_WINDOW_WIDTH = 200.f;

Level::Level() : World() {
	m_worldData = &m_levelData;

	m_avoidableTiles.insert(LevelDynamicTileID::SpikesTop);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesBottom);
	m_avoidableTiles.insert(LevelDynamicTileID::Falling);
	m_avoidableTiles.insert(LevelDynamicTileID::Fluid);
	m_avoidableTiles.insert(LevelDynamicTileID::Damaging);

	m_evilTiles.insert(LevelDynamicTileID::SpikesTop);
	m_evilTiles.insert(LevelDynamicTileID::SpikesBottom);
	m_evilTiles.insert(LevelDynamicTileID::Falling);
	m_evilTiles.insert(LevelDynamicTileID::Damaging);
}

void Level::loadCamera(LevelMainCharacter* mainChar) {
	if (abs(m_levelData.autoscrollerSpeed) > 0) {
		auto cam = new AutoscrollerCamera();
		cam->setCameraWindowWidth(CAMERA_WINDOW_WIDTH);
		cam->setCameraWindowHeight(CAMERA_WINDOW_HEIGHT);
		cam->setAutoscrollerSpeed(m_levelData.autoscrollerSpeed);
		cam->setFocusCenter(mainChar->getCenter(), true);
		cam->setCameraCenterX(WINDOW_WIDTH * 0.5f, true);
		cam->setLevelSize(sf::Vector2f(m_levelData.mapSize.x * TILE_SIZE_F, m_levelData.mapSize.y * TILE_SIZE_F));
		mainChar->setAutoscroller(cam);
		m_camera = cam;
	}
	else {
		auto cam = new SpeedupPullCamera();
		cam->setCameraWindowWidth(CAMERA_WINDOW_WIDTH);
		cam->setCameraWindowHeight(CAMERA_WINDOW_HEIGHT);
		cam->setFocusCenter(mainChar->getCenter(), true);
		m_camera = cam;
	}
}

Level::~Level() {
	delete m_camera;
	delete m_bossLevel;
}

void Level::dispose() {
	World::dispose();
	for (int i = 0; i < static_cast<int>(m_levelData.backgroundLayers.size()); i++) {
		m_levelData.backgroundLayers[i].dispose();
	}
	g_resourceManager->deleteLevelResources();
}

void Level::loadAfterMainChar(MainCharacter* mainChar) {
	LevelLoader loader;
	loader.loadAfterMainChar(m_levelData, dynamic_cast<LevelScreen*>(m_screen), this);
	loadCamera(dynamic_cast<LevelMainCharacter*>(mainChar));
}

bool Level::load(const std::string& id, WorldScreen* screen) {
	m_screen = screen;
	LevelReader reader;
	m_levelData.id = id;
	if (!reader.readLevel(id, m_levelData, m_screen->getCharacterCore())) {
		return false;
	}
	if (m_levelData.isBossLevel) {
		m_bossLevel = new BossLevel(screen);
		if (!m_bossLevel->loadLua(m_levelData.bossLevelPath)) {
			return false;
		}
	}
	if (m_levelData.isMagicLocked) {
		for (int type = static_cast<int>(SpellType::VOID) + 1; type < static_cast<int>(SpellType::MAX); type++) {
			setMagicLocked(static_cast<SpellType>(type));
		}
	}

	// adjust weather
	if (const WeatherData* weather = m_screen->getCharacterCore()->getWeather(id)) {
		m_levelData.weather = *weather;
	}

	// load level
	m_backgroundTileMap.load(m_levelData, m_levelData.backgroundTileLayers);
	m_lightedForegroundTileMap.load(m_levelData, m_levelData.lightedForegroundTileLayers);
	m_foregroundTileMap.load(m_levelData, m_levelData.foregroundTileLayers);

	g_resourceManager->loadLevelResources();

	return true;
}

void Level::loadForRenderTexture() {
	LevelLoader loader;
	loader.loadDynamicTiles(m_levelData, dynamic_cast<LevelScreen*>(m_screen));
	loader.loadLights(m_levelData, dynamic_cast<LevelScreen*>(m_screen));
	m_dynamicTiles = m_screen->getObjects(GameObjectType::_DynamicTile);
	m_movableTiles = m_screen->getObjects(GameObjectType::_MovableTile);
}

void Level::setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const {
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_levelData.mapRect.width - WINDOW_WIDTH / 2.f, m_camera->getCameraCenter().x));
	float camCenterY = std::max(WINDOW_HEIGHT / 2.f, std::min(m_levelData.mapRect.height - WINDOW_HEIGHT / 2.f, m_camera->getCameraCenter().y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
}

void Level::drawBackgroundLayers(sf::RenderTarget& target, const sf::RenderStates& states, const sf::Vector2f& focus) const {
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_camera->setFocusCenter(focus);

	// parallax background layers
	for (auto& layer : m_levelData.backgroundLayers) {
		// handle case for layer at infinity
		if (layer.getDistance() == -1.0f) {
			view.setCenter(0.5f * WINDOW_WIDTH, 0.5f * WINDOW_HEIGHT);
			target.setView(view);
		}
		else {
			float d = layer.getDistance();
			float ominoeseOffsetX = (WINDOW_WIDTH / 2) - (1 / d) * (WINDOW_WIDTH / 2);
			float viewCenterX = (std::max(WINDOW_WIDTH / 2.f, std::min(m_levelData.mapRect.width - WINDOW_WIDTH / 2.f, m_camera->getCameraCenter().x)) / d) + ominoeseOffsetX;
			float ominoeseOffsetY = (WINDOW_HEIGHT / 2) - (1 / d) * (WINDOW_HEIGHT / 2);
			float viewCenterY = (std::max(WINDOW_HEIGHT / 2.f, std::min(m_levelData.mapRect.height - WINDOW_HEIGHT / 2.f, m_camera->getCameraCenter().y)) / d) + ominoeseOffsetY;
			view.setCenter(viewCenterX, viewCenterY);
			target.setView(view);
		}
		layer.render(target, states);
	}
}

void Level::update(const sf::Time& frameTime) {
	World::update(frameTime);
	m_camera->update(frameTime);
}

bool Level::isInsideWorldRect(const sf::FloatRect& boundingBox) const {
	return !(boundingBox.top + boundingBox.height < m_levelData.mapRect.top || 
		boundingBox.top > m_levelData.mapRect.top + m_levelData.mapRect.height);
}

bool Level::collides(WorldCollisionQueryRecord& rec) const {
	World::collides(rec);
	// additional : check for collision with map rect (y axis)
	// a game object in a level can go until we don't see it anymore on the y axis. (further than only map rect collision)
	if (!isInsideWorldRect(rec.boundingBox)) {
		if (rec.collisionDirection == CollisionDirection::Up) {
			rec.safeTop = std::max(rec.safeTop, m_worldData->mapRect.top - rec.boundingBox.height);
		}
		if (rec.collisionDirection == CollisionDirection::Down) {
			rec.safeTop = std::min(rec.safeTop, m_worldData->mapRect.top + m_worldData->mapRect.height);
		}
		rec.collides = true;
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);
		if (tile == rec.excludedGameObject || !tile->isCollidable()) continue;
		if (rec.ignoreDynamicTiles && !(tile->isStrictlyCollidable())) continue;

		if (tile->isOneWay()) {
			if (rec.ignoreOnewayTiles || rec.collisionDirection != CollisionDirection::Down) continue;
			if (rec.excludedGameObject) {
				auto recBB = rec.excludedGameObject->getBoundingBox();
				if (recBB->top + recBB->height > tile->getPosition().y) continue;
			}
		}
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (epsIntersect(tileBB, rec.boundingBox)) {
			calculateCollisionLocations(rec, tileBB);
		}
	}

	for (GameObject* go : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);
		if (tile == rec.excludedGameObject || !tile->isCollidable()) continue;
		if (rec.ignoreDynamicTiles && !(tile->isStrictlyCollidable())) continue;

		if (tile->isOneWay()) {
			if (rec.ignoreOnewayTiles || rec.collisionDirection != CollisionDirection::Down) continue;
			if (rec.boundingBox.top + rec.boundingBox.height > tile->getPosition().y + 0.5f * TILE_SIZE_F) continue;
		}
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (epsIntersect(tileBB, rec.boundingBox)) {
			MovableGameObject* mob = dynamic_cast<MovableGameObject*>(tile);
			rec.movingParent = mob->getMovingParent(); // question: should we only take the moving parent if the max collision is this tile?
			calculateCollisionLocations(rec, tileBB);
		}
	}

	// MOB collision
	if (!rec.ignoreMobs) {
		collidesWithMobs(rec, false);
	}

	return rec.collides;
}

bool Level::collidesWithMobs(WorldCollisionQueryRecord& rec, bool isInitialQuery) const {
	if (isInitialQuery) {
		rec.collides = false;
	}
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto mainChar = m_screen->getObjects(GameObjectType::_LevelMainCharacter);

	for (auto enemy : *enemies) {
		const sf::FloatRect& mobBB = *enemy->getBoundingBox();
		LevelMovableGameObject* mob = dynamic_cast<LevelMovableGameObject*>(enemy);
		if (mob->isDead() || mob->isIgnoreDynamicTiles()) continue;
		if (epsIntersect(mobBB, rec.boundingBox)) {
			calculateCollisionLocations(rec, mobBB);
		}
	}

	const sf::FloatRect& mobBB = *(*mainChar)[0]->getBoundingBox();
	LevelMovableGameObject* mob = dynamic_cast<LevelMovableGameObject*>((*mainChar)[0]);
	if (!(mob->isDead() || mob->isIgnoreDynamicTiles()) && epsIntersect(mobBB, rec.boundingBox)) {
		calculateCollisionLocations(rec, mobBB);
	}

	return rec.collides;
}

bool Level::collidesWithMovableTiles(WorldCollisionQueryRecord& rec) const {
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile->isCollidable() && epsIntersect(tileBB, rec.boundingBox)) {
			return true;
		}
	}
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile->getDynamicTileID() != LevelDynamicTileID::Falling) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (epsIntersect(tileBB, rec.boundingBox)) {
			return true;
		}
	}
	return false;
}

void Level::collideWithDynamicTiles(Spell* spell, const sf::FloatRect& boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (epsIntersect(tileBB, boundingBox)) {
			tile->onHit(spell);
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (epsIntersect(tileBB, boundingBox)) {
			tile->onHit(spell);
		}
	}
}

bool Level::collidesWithAvoidableTiles(const sf::FloatRect& boundingBox) const {
	return collidesWithSpecificTiles(boundingBox, m_avoidableTiles);
}

bool Level::collidesWithEvilTiles(const sf::FloatRect& boundingBox) const {
	sf::FloatRect safeBB(boundingBox.left - 2, boundingBox.top - 2, boundingBox.width + 4, boundingBox.height + 4);
	return collidesWithSpecificTiles(safeBB, m_evilTiles);
}

bool Level::collidesWithSpecificTiles(const sf::FloatRect& boundingBox, const std::set<LevelDynamicTileID>& tiles) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (contains(tiles, tile->getDynamicTileID()) &&
			epsIntersect(tileBB, boundingBox)) {
			return true;
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (contains(tiles, tile->getDynamicTileID()) &&
			epsIntersect(tileBB, boundingBox)) {
			return true;
		}
	}
	return false;
}

const std::vector<GameObject*>* Level::getMovableTiles() const {
	return m_movableTiles;
}

const std::vector<GameObject*>* Level::getDynamicTiles() const {
	return m_dynamicTiles;
}

void Level::setBackgroundLayerColor(const sf::Color& color) const {
	for (auto& bgLayer : m_levelData.backgroundLayers) {
		const_cast<BackgroundLayer*>(&bgLayer)->setColor(color);
	}
}

void Level::raycast(RaycastQueryRecord& rec) const {
	raycastWorld(rec);
	if (rec.ignoreDynamicTiles) return;
	raycastDynamicTiles(rec);
}

void Level::raycastWorld(RaycastQueryRecord& rec) const {
	if (norm(rec.rayDirection) == 0) return;
	normalize(rec.rayDirection);

	// first checks
	bool ignoreX = std::abs(rec.rayDirection.x) < Epsilon;
	bool ignoreY = std::abs(rec.rayDirection.y) < Epsilon;

	float xDir = rec.rayDirection.x < 0 ? -1.f : 1.f;
	float yDir = rec.rayDirection.y < 0 ? -1.f : 1.f;

	rec.rayHit = rec.rayOrigin;
	sf::Vector2f oldRayHit = rec.rayHit;

	while (true) {
		// start at origin
		int tileX = static_cast<int>(floor(rec.rayHit.x / TILE_SIZE_F));
		int tileY = static_cast<int>(floor(rec.rayHit.y / TILE_SIZE_F));
		int potTileX = rec.rayHit.x == tileX * TILE_SIZE_F && !ignoreX && xDir == -1 ? tileX - 1 : tileX;
		int potTileY = rec.rayHit.y == tileY * TILE_SIZE_F && !ignoreY && yDir == -1 ? tileY - 1 : tileY;
		if (isTileCollidable(tileX, tileY) || isTileCollidable(potTileX, potTileY)) {
			break;
		}

		oldRayHit = rec.rayHit;

		if (ignoreX) {
			// only check Y
			tileY = rec.rayDirection.y > 0 ? tileY + 1 : tileY - 1;
			rec.rayHit.y = tileY * TILE_SIZE_F;
		}
		else if (ignoreY) {
			// only check X
			tileX = rec.rayDirection.x > 0 ? tileX + 1 : tileX - 1;
			rec.rayHit.x = tileX * TILE_SIZE_F;
		}
		else {
			// where does our ray hit?
			int nextTileX = rec.rayDirection.x > 0 ? tileX + 1 : rec.rayHit.x > tileX * TILE_SIZE_F ? tileX : tileX - 1;
			int nextTileY = rec.rayDirection.y > 0 ? tileY + 1 : rec.rayHit.y > tileY * TILE_SIZE_F ? tileY : tileY - 1;

			float distX = std::abs(nextTileX * TILE_SIZE_F - rec.rayHit.x);
			float distY = std::abs(nextTileY * TILE_SIZE_F - rec.rayHit.y);
			float relDistX = distX / std::abs(rec.rayDirection.x);
			float relDistY = distY / std::abs(rec.rayDirection.y);

			if (relDistX > relDistY) {
				// we will cross Y
				float angles = std::abs(angle(rec.rayDirection, sf::Vector2f(0.f, yDir)));
				float tanAngle = std::tan(angles);
				rec.rayHit.x = rec.rayHit.x + xDir * distY * tanAngle;
				rec.rayHit.y = nextTileY * TILE_SIZE_F;
			}
			else {
				// we will cross X
				float angles = std::abs(angle(rec.rayDirection, sf::Vector2f(xDir, 0.f)));
				float tanAngle = std::tan(angles);
				rec.rayHit.y = rec.rayHit.y + yDir * distX * tanAngle;
				rec.rayHit.x = nextTileX * TILE_SIZE_F;
			}
		}
	}
}

bool Level::isTileCollidable(int x, int y) const {
	return (y >= static_cast<int>(m_worldData->collidableTilePositions.size()) ||
		y < 0 || x < 0 ||
		x >= static_cast<int>(m_worldData->collidableTilePositions[y].size()) ||
		m_worldData->collidableTilePositions[y][x]);
}

void Level::raycastDynamicTiles(RaycastQueryRecord& rec) const {
	sf::Vector2f intersection;
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (!tile) continue;

		// check special mirror tiles
		if (tile->getDynamicTileID() == LevelDynamicTileID::Mirror && tile->getGameObjectState() == GameObjectState::Inactive) {
			if (lineBoxIntersection(rec.rayOrigin, rec.rayHit, *tile->getBoundingBox(), intersection)) {
				rec.rayHit = intersection;
				rec.mirrorTile = tile;
			}
		}

		if (!tile->isCollidable() || tile->isOneWay()) continue;
		if (lineBoxIntersection(rec.rayOrigin, rec.rayHit, *tile->getBoundingBox(), intersection)) {
			rec.rayHit = intersection;
			rec.mirrorTile = nullptr;
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (!tile || !tile->isCollidable()) continue;

		if (lineBoxIntersection(rec.rayOrigin, rec.rayHit, *tile->getBoundingBox(), intersection)) {
			rec.rayHit = intersection;
			rec.mirrorTile = nullptr;
		}
	}
}

void Level::collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect& boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (fastIntersect(tileBB, boundingBox)) {
			tile->onHit(mob);
		}
	}

	sf::FloatRect checkBB = boundingBox;
	checkBB.top -= 1.f;
	checkBB.left -= 1.f;
	checkBB.width += 2.f;
	checkBB.height += 2.f;
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (fastIntersect(tileBB, checkBB)) {
			tile->onHit(mob);
		}
	}
}

void Level::executeBossEnding(bool win) {
	if (m_bossLevel == nullptr) return;

	if (win) {
		m_bossLevel->executeOnWin();
	}
	else {
		m_bossLevel->executeOnLose();
	}
}

void Level::setMagicLocked(SpellType type) {
	m_lockedMagicTypes.insert(type);
}

bool Level::isMagicLocked(SpellType type) {
	return contains(m_lockedMagicTypes, type);
}

const std::set<SpellType>& Level::getLockedMagic() const {
	return m_lockedMagicTypes;
}
