#include "Level/Level.h"
#include "Screen.h"

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

	tileWidth = static_cast<float>(m_levelData.tileSize.x);
	tileHeight = static_cast<float>(m_levelData.tileSize.y);
	g_resourceManager->loadLevelResources();
	return true;
}

void Level::loadForRenderTexture(Screen* screen) {
	LevelLoader loader;
	loader.loadDynamicTiles(m_levelData, screen, this);
	loader.loadLights(m_levelData, screen);
	m_dynamicTiles = screen->getObjects(GameObjectType::_DynamicTile);
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
	sf::FloatRect dummyRect = boundingBox;
	dummyRect.left = right ? dummyRect.left + stepSize : dummyRect.left - stepSize;
	if (collides(dummyRect, nullptr, ignoreDynamicTiles)) {
		return false;
	}
	for (float y = boundingBox.top; y < boundingBox.top + jumpHeight; /* don't increment y here */) {
		y = std::min(boundingBox.top + jumpHeight, y + tileHeight);
		dummyRect.top = y;
		if (collides(dummyRect, nullptr, ignoreDynamicTiles)) {
			return false;
		}
	}
	return true;
}

bool Level::collidesAfterJump(const sf::FloatRect& boundingBox, float jumpHeight, bool right, bool ignoreDynamicTiles) const {
	sf::FloatRect dummyRect = boundingBox;
	for (float y = boundingBox.top; y > boundingBox.top - jumpHeight; /* don't decrement y here */) {
		y = std::max(boundingBox.top - jumpHeight, y - tileHeight);
		dummyRect.top = y;
		if (collides(dummyRect, nullptr, ignoreDynamicTiles)) {
			return true;
		}
	}

	// depending on left or right, calculate one step left or right
	dummyRect.left = right ? dummyRect.left + 10.f : dummyRect.left - 10.f;
	if (collides(dummyRect, nullptr, ignoreDynamicTiles)) {
		return true;
	}
	return false;
}

bool Level::collides(const sf::FloatRect& boundingBox, const GameObject* exclude, bool ignoreDynamicTiles, bool ignoreMobs) const {
	// additional : check for collision with map rect (y axis)
	if (boundingBox.top - boundingBox.height < m_levelData.mapRect.top || boundingBox.top > m_levelData.mapRect.top + m_levelData.mapRect.height) {
		return true;
	}
	if (World::collides(boundingBox)) {
		return true;
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles) {
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(go);
		if (ignoreDynamicTiles && !(tile->getIsStrictlyCollidable())) continue;
		if (tile != nullptr && tile != exclude && tile->getIsCollidable() && tile->getBoundingBox()->intersects(boundingBox)) {
			return true;
		}
	}

	if (ignoreMobs) {
		return false;
	}

	// MOB collision
	return collidesWithMobs(boundingBox);
}

bool Level::collidesWithMobs(const sf::FloatRect& boundingBox) const {
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto mainChar = m_screen->getObjects(GameObjectType::_LevelMainCharacter);

	for (auto enemy : *enemies) {
		if (enemy->getBoundingBox()->intersects(boundingBox)) {
			return true;
		}
	}

	if ((*mainChar)[0]->getBoundingBox()->intersects(boundingBox)) {
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
	return false;
}

void Level::collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect* boundingBox) const {
	for (auto& it : *m_dynamicTiles) {
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