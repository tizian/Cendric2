#include "Level/Level.h"
#include "Screen.h"
#include "Level/DynamicTiles/MovingTile.h"

using namespace std;

Level::Level() : World() {
	m_worldData = &m_levelData;
	m_camera = new SpeedupPullCamera();
	m_camera->setCameraWindowWidth(CAMERA_WINDOW_WIDTH);
	m_camera->setCameraWindowHeight(CAMERA_WINDOW_HEIGHT);
}

Level::~Level() {
	delete m_camera;
}

void Level::dispose() {
	World::dispose();
	for (int i = 0; i < static_cast<int>(m_levelData.backgroundLayers.size()); i++) {
		m_levelData.backgroundLayers[i].dispose();
	}
	g_resourceManager->deleteLevelResources();
}

void Level::loadAfterMainChar(Screen* screen) {
	LevelLoader loader;
	m_screen = screen;
	loader.loadAfterMainChar(m_levelData, screen, this);
}

bool Level::load(const std::string& id) {
	LevelReader reader;
	if (!reader.readLevel(id, m_levelData)) {
		return false;
	}

	m_levelData.id = id;
	// load level
	m_backgroundTileMap.load(m_levelData, m_levelData.backgroundTileLayers);
	m_lightedForegroundTileMap.load(m_levelData, m_levelData.lightedForegroundTileLayers);
	m_foregroundTileMap.load(m_levelData, m_levelData.foregroundTileLayers);

	g_resourceManager->loadLevelResources();
	return true;
}

void Level::loadForRenderTexture(Screen* screen) {
	LevelLoader loader;
	loader.loadDynamicTiles(m_levelData, screen, this);
	loader.loadLights(m_levelData, screen);
	m_dynamicTiles = screen->getObjects(GameObjectType::_DynamicTile);
	m_movableTiles = screen->getObjects(GameObjectType::_MovableTile);
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
	for (int i = 0; i < m_levelData.backgroundLayers.size(); i++) {
		// handle case for layer at infinity
		if (m_levelData.backgroundLayers[i].getDistance() == -1.0f) {
			view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
			target.setView(view);
		}
		else {
			float d = m_levelData.backgroundLayers[i].getDistance();
			float ominoeseOffsetX = (WINDOW_WIDTH / 2) - (1 / d) * (WINDOW_WIDTH / 2);
			float viewCenterX = (std::max(WINDOW_WIDTH / 2.f, std::min(m_levelData.mapRect.width - WINDOW_WIDTH / 2.f, m_camera->getCameraCenter().x)) / d) + ominoeseOffsetX;
			float ominoeseOffsetY = (WINDOW_HEIGHT / 2) - (1 / d) * (WINDOW_HEIGHT / 2);
			float viewCenterY = (std::max(WINDOW_HEIGHT / 2.f, std::min(m_levelData.mapRect.height - WINDOW_HEIGHT / 2.f, m_camera->getCameraCenter().y)) / d) + ominoeseOffsetY;
			view.setCenter(viewCenterX, viewCenterY);
			target.setView(view);
		}
		m_levelData.backgroundLayers[i].render(target, states);
	}
}

void Level::update(const sf::Time& frameTime) {
	World::update(frameTime);
	m_camera->update(frameTime);
}

bool Level::collides(WorldCollisionQueryRecord& rec) const {
	World::collides(rec);
	// additional : check for collision with map rect (y axis)
	// a game object in a level can go until we don't see it anymore on the y axis. (further than only map rect collision)
	if (rec.boundingBox.top + rec.boundingBox.height < m_levelData.mapRect.top || rec.boundingBox.top > m_levelData.mapRect.top + m_levelData.mapRect.height) {
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

		if (rec.ignoreDynamicTiles && !(tile->getIsStrictlyCollidable())) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile != rec.excludedGameObject && tile->getIsCollidable() && epsIntersect(tileBB, rec.boundingBox)) {
			calculateCollisionLocations(rec, tileBB);
		}
	}

	for (GameObject* go : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);

		if (rec.ignoreDynamicTiles && !(tile->getIsStrictlyCollidable())) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile != rec.excludedGameObject && tile->getIsCollidable() && epsIntersect(tileBB, rec.boundingBox)) {
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
		if (tile->getIsCollidable() && epsIntersect(tileBB, rec.boundingBox)) {
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

bool Level::collidesWithDynamicTiles(const sf::FloatRect& boundingBox, const std::set<LevelDynamicTileID>& tiles) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tiles.find(tile->getDynamicTileID()) != tiles.end() &&
			epsIntersect(tileBB, boundingBox)) {
			return true;
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tiles.find(tile->getDynamicTileID()) != tiles.end() &&
			epsIntersect(tileBB, boundingBox)) {
			return true;
		}
	}
	return false;
}

void Level::collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect& boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tileBB.intersects(boundingBox)) {
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
		if (tileBB.intersects(checkBB)) {
			tile->onHit(mob);
		}
	}
}

LevelExitData* Level::checkLevelExit(const sf::FloatRect& boundingBox) const {
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		for (auto it : m_levelData.levelExits) {
			if (boundingBox.intersects(it.levelExitRect)) {
				LevelExitData* exit = new LevelExitData(it);
				return exit;
			}
		}
	}

	return nullptr;
}