#include "Level.h"
#include "Screen.h"

using namespace std;

Level::Level()
{
	m_camera = new SpeedupPullCamera();
	m_camera->setCameraWindowWidth(CAMERA_WINDOW_WIDTH);
	m_camera->setCameraWindowHeight(CAMERA_WINDOW_HEIGHT);
}

Level::~Level()
{
	delete m_camera;
	dispose();
}

void Level::dispose()
{
	// no need to dispose both tile maps (fg + bg), as they share their resource
	m_backgroundTileMap.dispose();
	for (int i = 0; i < m_levelData.backgroundLayers.size(); i++)
	{
		m_levelData.backgroundLayers[i].dispose();
	}
	g_resourceManager->deleteLevelResources();
}

void Level::loadAfterMainChar(Screen* screen)
{
	LevelLoader loader;
	loader.loadEnemies(m_levelData, screen, this);
	loader.loadLevelItems(m_levelData, screen);
	loader.loadChestTiles(m_levelData, screen, this);
}

bool Level::load(const std::string& id) 
{
	LevelReader reader;
	if (!reader.readLevel(id, m_levelData))
	{
		return false;
	}

	m_levelData.id = id;
	// load level
	m_backgroundTileMap.load(m_levelData.tileSetPath, m_levelData.tileSize, m_levelData.backgroundTileLayers, m_levelData.mapSize.x, m_levelData.mapSize.y);
	m_foregroundTileMap.load(m_levelData.tileSetPath, m_levelData.tileSize, m_levelData.foregroundTileLayers, m_levelData.mapSize.x, m_levelData.mapSize.y);

	tileWidth = static_cast<float>(m_levelData.tileSize.x);
	tileHeight = static_cast<float>(m_levelData.tileSize.y);
	g_resourceManager->loadLevelResources();
	return true;
}

void Level::loadForRenderTexture(Screen* screen)
{
	LevelLoader loader;
	loader.loadDynamicTiles(m_levelData, screen, this);
	loader.loadLights(m_levelData, screen);
	m_dynamicTiles = screen->getObjects(GameObjectType::_DynamicTile);
}
void Level::drawBackground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_camera->setFocusCenter(center);

	// parallax background layers
	for (int i = 0; i < m_levelData.backgroundLayers.size(); i++)
	{
		// handle case for layer at infinity
		if (m_levelData.backgroundLayers[i].getDistance() == -1.0f)
		{
			view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
			target.setView(view);
		}
		else
		{
			float d = m_levelData.backgroundLayers[i].getDistance();
			float ominoeseOffsetX = (WINDOW_WIDTH / 2) - (1 / d) * (WINDOW_WIDTH / 2);
			float viewCenterX = (std::max(WINDOW_WIDTH / 2.f, std::min(m_levelData.levelRect.width - WINDOW_WIDTH / 2.f, m_camera->getCameraCenter().x)) / d) + ominoeseOffsetX;
			float ominoeseOffsetY = (WINDOW_HEIGHT / 2) - (1 / d) * (WINDOW_HEIGHT / 2);
			float viewCenterY = (std::max(WINDOW_HEIGHT / 2.f, std::min(m_levelData.levelRect.height - WINDOW_HEIGHT / 2.f, m_camera->getCameraCenter().y)) / d) + ominoeseOffsetY;
			view.setCenter(viewCenterX, viewCenterY);
			target.setView(view);
		}
		m_levelData.backgroundLayers[i].render(target, states);
	}

	// tilemap
	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_levelData.levelRect.width - WINDOW_WIDTH / 2.f, m_camera->getCameraCenter().x));
	float camCenterY = std::max(WINDOW_HEIGHT / 2.f, std::min(m_levelData.levelRect.height - WINDOW_HEIGHT / 2.f, m_camera->getCameraCenter().y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
	m_backgroundTileMap.draw(target, states);

	// dynamic tiles
	for (auto &it : (*m_dynamicTiles))
	{
		it->render(target);
	}
}

void Level::drawForeground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const
{
	m_foregroundTileMap.draw(target, states);
	// dynamic tiles
	for (auto &it : (*m_dynamicTiles))
	{
		it->renderAfterForeground(target);
	}
}

const sf::FloatRect& Level::getLevelRect() const
{
	return m_levelData.levelRect;
}

bool Level::collidesX(const sf::FloatRect& boundingBox, const DynamicTile* exclude) const
{
	// check for collision with level rect
	if (boundingBox.left < m_levelData.levelRect.left || boundingBox.left + boundingBox.width > m_levelData.levelRect.left + m_levelData.levelRect.width)
	{
		return true;
	}

	// normalize bounding box values so they match our collision grid. Wondering about the next two lines? Me too. We just don't want to floor values that are exactly on the boundaries. But only those that are down and right.
	int bottomY = static_cast<int>(floor((boundingBox.top + boundingBox.height) / tileHeight) == (boundingBox.top + boundingBox.height) / tileHeight ? (boundingBox.top + boundingBox.height) / tileHeight - 1 : floor((boundingBox.top + boundingBox.height) / tileHeight));
	int rightX = static_cast<int>(floor((boundingBox.left + boundingBox.width) / tileWidth) == (boundingBox.left + boundingBox.width) / tileWidth ? (boundingBox.left + boundingBox.width) / tileWidth - 1 : floor((boundingBox.left + boundingBox.width) / tileWidth));
	sf::Vector2i topLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i topRight(rightX, static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i bottomLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), bottomY);
	sf::Vector2i bottomRight(rightX, bottomY);

	// check left side
	int x = topLeft.x;
	for (int y = topLeft.y; y <= bottomLeft.y; y++)
	{
		if (y > m_levelData.collidableTilePositions.size() || y < 0 || x < 0 || x > m_levelData.collidableTilePositions[y].size())
		{
			// check for out of range (happens seldom because of rounding problems above)
			return true;
		}
		if (m_levelData.collidableTilePositions[y][x])
		{
			return true;
		}
	}

	// check right side
	x = topRight.x;
	for (int y = topRight.y; y <= bottomRight.y; y++)
	{
		if (y > m_levelData.collidableTilePositions.size() || y < 0 || x < 0 || x > m_levelData.collidableTilePositions[y].size())
		{
			// check for out of range (happens seldom because of rounding problems above)
			return true;
		}
		if (m_levelData.collidableTilePositions[y][x])
		{
			return true;
		}
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles)
	{
		if (!go->isViewable()) continue;
		DynamicTile* tile = dynamic_cast<DynamicTile*>(go);
		if (tile != nullptr && tile != exclude && tile->getIsCollidable() && tile->getBoundingBox()->intersects(boundingBox))
		{
			return true;
		}
	}

	return false;
}

bool Level::collidesLevelBottom(const sf::FloatRect& boundingBox) const
{
	// check for collision with level rect
	if (boundingBox.top + boundingBox.height > m_levelData.levelRect.top + m_levelData.levelRect.height)
	{
		return true;
	}
	return false;
}

bool Level::collidesLevelCeiling(const sf::FloatRect& boundingBox) const
{
	// check for collision with level rect
	if (boundingBox.top < m_levelData.levelRect.top)
	{
		return true;
	}
	return false;
}

bool Level::fallsDeep(const sf::FloatRect& boundingBox, float jumpHeight, bool right, float stepSize) const
{
	sf::FloatRect dummyRect = boundingBox;
	dummyRect.left = right ? dummyRect.left + stepSize : dummyRect.left - stepSize;
	if (collidesY(dummyRect) || collidesX(dummyRect))
	{
		return false;
	}
	for (float y = boundingBox.top; y < boundingBox.top + jumpHeight; /* don't increment y here */)
	{
		y = std::min(boundingBox.top + jumpHeight, y + tileHeight);
		dummyRect.top = y;
		if (collidesY(dummyRect) || collidesX(dummyRect))
		{
			return false;
		}
	}
	return true;
}

bool Level::collidesAfterJump(const sf::FloatRect& boundingBox, float jumpHeight, bool right) const
{
	sf::FloatRect dummyRect = boundingBox;
	for (float y = boundingBox.top; y > boundingBox.top - jumpHeight; /* don't decrement y here */)
	{
		y = std::max(boundingBox.top - jumpHeight, y - tileHeight);
		dummyRect.top = y;
		if (collidesY(dummyRect) || collidesX(dummyRect))
		{
			return true;
		}
	}

	// depending on left or right, calculate one step left or right
	dummyRect.left = right ? dummyRect.left + 10.f : dummyRect.left - 10.f;
	if (collidesX(dummyRect))
	{
		return true;
	}
	return false;
}

bool Level::collidesY(const sf::FloatRect& boundingBox, const DynamicTile* exclude) const
{
	// check for collision with level rect
	if (boundingBox.top < m_levelData.levelRect.top || boundingBox.top + boundingBox.height > m_levelData.levelRect.top + m_levelData.levelRect.height)
	{
		return true;
	}

	// normalize bounding box values so they match our collision grid. Wondering about the next two lines? Me too. We just don't want to floor values that are exactly on the boundaries. But only those that are down and right.
	int bottomY = static_cast<int>(floor((boundingBox.top + boundingBox.height) / tileHeight) == (boundingBox.top + boundingBox.height) / tileHeight ? (boundingBox.top + boundingBox.height) / tileHeight - 1 : floor((boundingBox.top + boundingBox.height) / tileHeight));
	int rightX = static_cast<int>(floor((boundingBox.left + boundingBox.width) / tileWidth) == (boundingBox.left + boundingBox.width) / tileWidth ? (boundingBox.left + boundingBox.width) / tileWidth - 1 : floor((boundingBox.left + boundingBox.width) / tileWidth));
	sf::Vector2i topLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i topRight(rightX, static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i bottomLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), bottomY);
	sf::Vector2i bottomRight(rightX, bottomY);

	// check top side
	int y = topLeft.y;
	for (int x = topLeft.x; x <= topRight.x; x++)
	{
		if (y > m_levelData.collidableTilePositions.size() || y < 0 || x < 0 || x > m_levelData.collidableTilePositions[y].size())
		{
			// check for out of range (happens seldom because of rounding problems above)
			return true;
		}
		if (m_levelData.collidableTilePositions[y][x])
		{
			return true;
		}
	}

	// check bottom side
	y = bottomLeft.y;
	for (int x = bottomLeft.x; x <= bottomRight.x; x++)
	{
		if (y > m_levelData.collidableTilePositions.size() || y < 0 || x < 0 || x > m_levelData.collidableTilePositions[y].size())
		{
			// check for out of range (happens seldom because of rounding problems above)
			return true;
		}
		if (m_levelData.collidableTilePositions[y][x])
		{
			return true;
		}
	}

	// check collidable dynamic tiles
	for (GameObject* go : *m_dynamicTiles)
	{
		if (!go->isViewable()) continue;
		DynamicTile* tile = dynamic_cast<DynamicTile*>(go);
		if (tile != nullptr && tile != exclude && tile->getIsCollidable() && tile->getBoundingBox()->intersects(boundingBox))
		{
			return true;
		}
	}

	return false;
}

bool Level::collidesY(const sf::FloatRect& boundingBox) const
{
	return collidesY(boundingBox, nullptr);
}

bool Level::collidesX(const sf::FloatRect& boundingBox) const
{
	return collidesX(boundingBox, nullptr);
}

float Level::getGround(const sf::FloatRect& boundingBox) const
{
	// check if ground is level ground
	if (boundingBox.top + boundingBox.height > m_levelData.levelRect.top + m_levelData.levelRect.height)
	{
		return (m_levelData.levelRect.top + m_levelData.levelRect.height) - boundingBox.height;
	}

	// then, a collidable tile in the grid must be the ground
	float tileHeight = static_cast<float>(m_levelData.tileSize.y);
	int y = static_cast<int>(floor((boundingBox.top + boundingBox.height) / tileHeight));

	return (y * tileHeight) - boundingBox.height;
}

float Level::getCeiling(const sf::FloatRect& boundingBox) const
{
	// check if ceiling is level ceiling
	if (boundingBox.top < m_levelData.levelRect.top)
	{
		return m_levelData.levelRect.top;
	}

	// then, a collidable tile in the grid must be the ceiling
	float tileHeight = static_cast<float>(m_levelData.tileSize.y);
	int y = static_cast<int>(ceil((boundingBox.top) / tileHeight));

	return y * tileHeight;
}

void Level::collideWithDynamicTiles(Spell* spell, const sf::FloatRect* boundingBox) const
{
	size_t size = m_dynamicTiles->size(); // Note: this loop type allows objects to be added to the back of the list while iterating over it.
	for (size_t i = 0; i < size; ++i)
	{
		DynamicTile* tile = dynamic_cast<DynamicTile*>(m_dynamicTiles->at(i));
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox)))
		{
			tile->onHit(spell);
		}
	}
}

void Level::collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect* boundingBox) const
{
	for (auto& it : *m_dynamicTiles)
	{
		DynamicTile* tile = dynamic_cast<DynamicTile*>(it);
		if (tile != nullptr && (tile->getBoundingBox()->intersects(*boundingBox)))
		{
			tile->onHit(mob);
		}
	}
}

LevelExitBean* Level::checkLevelExit(const sf::FloatRect& boundingBox) const
{
	if (g_inputController->isKeyJustPressed(Key::Up))
	{
		for (auto it : m_levelData.levelExits)
		{
			if (boundingBox.intersects(it.levelExitRect))
			{
				LevelExitBean* exit = new LevelExitBean();
				exit->mapID = it.mapID;
				exit->mapSpawnPoint = it.mapSpawnPoint;
				return exit;
			}
		}
	}

	return nullptr;
}

const std::string& Level::getID() const
{
	return m_levelData.id;
}

const std::string& Level::getMusicPath() const
{
	return m_levelData.musicPath;
}

float Level::getDimming() const
{
	return m_levelData.dimming;
}