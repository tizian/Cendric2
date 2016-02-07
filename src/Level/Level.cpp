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

bool Level::fallsDeep(const sf::FloatRect& boundingBox, float jumpHeight, bool right, float stepSize, bool ignoreDynamicTiles) const {
	WorldCollisionQueryRecord rec;
	rec.boundingBox = boundingBox;
	rec.ignoreDynamicTiles = ignoreDynamicTiles;
	rec.boundingBox.left = right ? rec.boundingBox.left + stepSize : rec.boundingBox.left - stepSize;
	if (collides(rec)) {
		return false;
	}
	for (float y = boundingBox.top; y < boundingBox.top + jumpHeight; /* don't increment y here */) {
		y = std::min(boundingBox.top + jumpHeight, y + TILE_SIZE_F);
		rec.boundingBox.top = y;
		if (collides(rec)) {
			return false;
		}
	}
	return true;
}

bool Level::collidesAfterJump(const sf::FloatRect& boundingBox, float jumpHeight, bool right, bool ignoreDynamicTiles) const {
	WorldCollisionQueryRecord rec;
	rec.boundingBox = boundingBox;
	rec.ignoreDynamicTiles = ignoreDynamicTiles;
	for (float y = boundingBox.top; y > boundingBox.top - jumpHeight; /* don't decrement y here */) {
		y = std::max(boundingBox.top - jumpHeight, y - TILE_SIZE_F);
		rec.boundingBox.top = y;
		if (collides(rec)) {
			return true;
		}
	}

	// depending on left or right, calculate one step left or right
	rec.boundingBox.left = right ? rec.boundingBox.left + 10.f : rec.boundingBox.left - 10.f;
	if (collides(rec)) {
		return true;
	}
	return false;
}

bool Level::collides(WorldCollisionQueryRecord& rec) const {
	// additional : check for collision with map rect (y axis)
	// a game object in a level can go until we don't see it anymore on the y axis. (further than only map rect collision)
	if (rec.boundingBox.top - rec.boundingBox.height < m_levelData.mapRect.top || rec.boundingBox.top > m_levelData.mapRect.top + m_levelData.mapRect.height) {
		if (rec.collisionDirection == CollisionDirection::Up) {
			rec.saveTop = m_worldData->mapRect.top - rec.boundingBox.height;
		}
		if (rec.collisionDirection == CollisionDirection::Down) {
			rec.saveTop = m_worldData->mapRect.top + m_worldData->mapRect.height;
		}
		return true;
	}
	if (World::collides(rec)) {
		return true;
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);

		if (rec.ignoreDynamicTiles && !(tile->getIsStrictlyCollidable())) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile != rec.excludedGameObject && tile->getIsCollidable() && tileBB.intersects(rec.boundingBox)) {
			if (rec.collisionDirection == CollisionDirection::Right) {
				rec.saveLeft = tileBB.left - rec.boundingBox.width;
			}
			if (rec.collisionDirection == CollisionDirection::Left) {
				rec.saveLeft = tileBB.left + tileBB.width;
			}
			if (rec.collisionDirection == CollisionDirection::Up) {
				rec.saveTop = tileBB.top + tileBB.height;
			}
			if (rec.collisionDirection == CollisionDirection::Down) {
				rec.saveTop = tileBB.top - rec.boundingBox.height;
			}
			return true;
		}
	}
	// check collidable movable tiles
	for (GameObject* go : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);
		if (tile->getDynamicTileID() == LevelDynamicTileID::Moving) continue;
		if (rec.ignoreDynamicTiles && !(tile->getIsStrictlyCollidable())) continue;
		const sf::FloatRect& tileBB = *tile->getBoundingBox();
		if (tile != rec.excludedGameObject && tile->getIsCollidable() && tileBB.intersects(rec.boundingBox)) {
			if (MovableGameObject* mob = dynamic_cast<MovableGameObject*>(tile)) {
				// used by shiftable blocks and unstable blocks
				rec.gainedRelativeVelocity = mob->getRelativeVelocity();
			}
			if (rec.collisionDirection == CollisionDirection::Right) {
				rec.saveLeft = tileBB.left - rec.boundingBox.width;
			}
			if (rec.collisionDirection == CollisionDirection::Left) {
				rec.saveLeft = tileBB.left + tileBB.width;
			}
			if (rec.collisionDirection == CollisionDirection::Up) {
				rec.saveTop = tileBB.top + tileBB.height;
			}
			if (rec.collisionDirection == CollisionDirection::Down) {
				rec.saveTop = tileBB.top - rec.boundingBox.height;
			}
			return true;
		}
	}

	// check for moving platforms
	if (rec.checkMovingPlatforms) {
		for (GameObject* go : *m_movableTiles) {
			MovingTile* movingTile = dynamic_cast<MovingTile*>(go);

			if (movingTile == nullptr || movingTile == rec.excludedGameObject) continue;

			sf::FloatRect checkBB = *movingTile->getBoundingBox();

			if (!checkBB.intersects(rec.boundingBox)) continue;

			if (!rec.upsideDown) {
				float yPos = rec.boundingBox.top + rec.boundingBox.height;
				float movingTileY = checkBB.top;
				if (yPos > movingTileY && yPos < movingTileY + 20.f) {
					rec.gainedRelativeVelocity = movingTile->getRelativeVelocity();
					rec.saveTop = movingTileY - rec.boundingBox.height;
					return true;
				}
			}
			else {
				float yPos = rec.boundingBox.top;
				float movingTileY = checkBB.top + checkBB.height;
				if (yPos < movingTileY && yPos > movingTileY - 20.f) {
					rec.gainedRelativeVelocity = movingTile->getRelativeVelocity();
					rec.saveTop = movingTileY;
					return true;
				}
			}
		}
	}

	if (rec.ignoreMobs) {
		return false;
	}

	// MOB collision
	return collidesWithMobs(rec);
}

bool Level::collidesWithMobs(WorldCollisionQueryRecord& rec) const {
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto mainChar = m_screen->getObjects(GameObjectType::_LevelMainCharacter);

	for (auto enemy : *enemies) {
		const sf::FloatRect& mobBB = *enemy->getBoundingBox();
		if (mobBB.intersects(rec.boundingBox)) {
			if (rec.collisionDirection == CollisionDirection::Right) {
				rec.saveLeft = mobBB.left - rec.boundingBox.width;
			}
			if (rec.collisionDirection == CollisionDirection::Left) {
				rec.saveLeft = mobBB.left + mobBB.width;
			}
			if (rec.collisionDirection == CollisionDirection::Up) {
				rec.saveTop = mobBB.top + mobBB.height;
			}
			if (rec.collisionDirection == CollisionDirection::Down) {
				rec.saveTop = mobBB.top - rec.boundingBox.height;
			}
			return true;
		}
	}

	const sf::FloatRect& mobBB = *(*mainChar)[0]->getBoundingBox();
	if (mobBB.intersects(rec.boundingBox)) {
		if (rec.collisionDirection == CollisionDirection::Right) {
			rec.saveLeft = mobBB.left - rec.boundingBox.width;
		}
		if (rec.collisionDirection == CollisionDirection::Left) {
			rec.saveLeft = mobBB.left + mobBB.width;
		}
		if (rec.collisionDirection == CollisionDirection::Up) {
			rec.saveTop = mobBB.top + mobBB.height;
		}
		if (rec.collisionDirection == CollisionDirection::Down) {
			rec.saveTop = mobBB.top - rec.boundingBox.height;
		}
		return true;
	}
	return false;
}

void Level::collideWithDynamicTiles(Spell* spell, const sf::FloatRect* boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox))) {
			tile->onHit(spell);
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox))) {
			tile->onHit(spell);
		}
	}
}

bool Level::collidesWithDynamicTiles(const sf::FloatRect* boundingBox, const std::set<LevelDynamicTileID>& tiles) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr &&
			tiles.find(tile->getDynamicTileID()) != tiles.end() &&
			(tile->getBoundingBox()->intersects(*boundingBox))) {
			return true;
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr &&
			tiles.find(tile->getDynamicTileID()) != tiles.end() &&
			(tile->getBoundingBox()->intersects(*boundingBox))) {
			return true;
		}
	}
	return false;
}

void Level::collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect* boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox))) {
			tile->onHit(mob);
		}
	}
	for (auto& it : *m_movableTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox))) {
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