#include "TileMap.h"

using namespace std;

bool TileMap::load(const WorldData& data, const std::vector<std::vector<int> >& layers) {
	if (layers.empty()) return false;
	m_tilesetPath = data.tileSetPath;
	m_tileset = g_resourceManager->getTexture(data.tileSetPath);
	m_tilesize = sf::Vector2i(TILE_SIZE, TILE_SIZE);
	int width = static_cast<int>(data.mapRect.width / TILE_SIZE_F);
	int height = static_cast<int>(data.mapRect.height / TILE_SIZE_F);

	// animated tileIDs
	std::set<int> animatedTileIDs;
	for (auto animatedTile : data.animatedTiles) {
		animatedTileIDs.insert(animatedTile.tileID);
	}

	m_layers.clear();

	for (int count = 0; count < static_cast<int>(layers.size()); count++) {
		sf::VertexArray layer;

		layer.setPrimitiveType(sf::Quads);
		layer.resize(width * height * 4);

		m_animatedTiles.insert({ count, std::vector<AnimatedTile*>() });

		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				int tileNumber = layers[count][i + j * width];

				if (tileNumber == 0) {
					// there is no tile
					continue;
				}
				if (animatedTileIDs.find(tileNumber) != animatedTileIDs.end()) {
					readAnimatedTile(tileNumber, count, i, j, data);
					continue;
				}

				// we start with tiles at 1 in the .tmx format, but need 0 here
				tileNumber--;

				int tu = tileNumber % (m_tileset->getSize().x / (TILE_SIZE + 2 * TILE_BORDER));
				int tv = tileNumber / (m_tileset->getSize().x / (TILE_SIZE + 2 * TILE_BORDER));

				sf::Vertex *quad = &layer[(i + j * width) * 4];

				quad[0].position = sf::Vector2f(i * TILE_SIZE_F, j * TILE_SIZE_F);
				quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE_F, j * TILE_SIZE_F);
				quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE_F, (j + 1) * TILE_SIZE_F);
				quad[3].position = sf::Vector2f(i * TILE_SIZE_F, (j + 1) * TILE_SIZE_F);

				quad[0].texCoords = sf::Vector2f(tu * (TILE_SIZE_F + 2 * TILE_BORDER) + TILE_BORDER, tv * (TILE_SIZE_F + 2 * TILE_BORDER) + TILE_BORDER);
				quad[1].texCoords = sf::Vector2f((tu + 1) * (TILE_SIZE_F + 2 * TILE_BORDER) - TILE_BORDER, tv * (TILE_SIZE_F + 2 * TILE_BORDER) + TILE_BORDER);
				quad[2].texCoords = sf::Vector2f((tu + 1) * (TILE_SIZE_F + 2 * TILE_BORDER) - TILE_BORDER, (tv + 1) * (TILE_SIZE_F + 2 * TILE_BORDER) - TILE_BORDER);
				quad[3].texCoords = sf::Vector2f(tu * (TILE_SIZE_F + 2 * TILE_BORDER) + TILE_BORDER, (tv + 1) * (TILE_SIZE_F + 2 * TILE_BORDER) - TILE_BORDER);
			}
		}

		m_layers.push_back(layer);
	}

	return true;
}

void TileMap::readAnimatedTile(int tileNumber, int layerNr, int i, int j, const WorldData& data) {
	for (auto& tile : data.animatedTiles) {
		if (tile.tileID == tileNumber) {

			AnimatedTile* animatedTile = new AnimatedTile();
			
			Animation* idleAnimation = new Animation(tile.frames.at(0).second);
			idleAnimation->setSpriteSheet(m_tileset);

			for (auto& frame : tile.frames) {
				int frameTileID = frame.first - 1;
				int tu = frameTileID % (m_tileset->getSize().x / (TILE_SIZE + 2 * TILE_BORDER));
				int tv = frameTileID / (m_tileset->getSize().x / (TILE_SIZE + 2 * TILE_BORDER));

				idleAnimation->addFrame(sf::IntRect(
					tu * (TILE_SIZE + 2 * TILE_BORDER) + TILE_BORDER, 
					tv * (TILE_SIZE + 2 * TILE_BORDER) + TILE_BORDER, 
					TILE_SIZE, 
					TILE_SIZE));
			}
			
			animatedTile->addAnimation(GameObjectState::Idle, idleAnimation);

			sf::Vector2f position(i * TILE_SIZE_F, j * TILE_SIZE_F);

			// initial values
			animatedTile->playCurrentAnimation(true);
			animatedTile->setPosition(position);
			animatedTile->setState(GameObjectState::Idle);

			m_animatedTiles[layerNr].push_back(animatedTile);
			
			break;
		}
	}
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = m_tileset;
	for (int i = 0; i < m_layers.size(); i++) {
		target.draw(m_layers[i], states);
		for (auto& tile : m_animatedTiles.at(i)) {
			target.draw(tile->getAnimatedSprite(), states);
		}
	}
}

void TileMap::update(const sf::Time& frameTime) {
	for (auto& it : m_animatedTiles) {
		for (auto& tile : it.second) {
			tile->update(frameTime);
		}
	}
}

void TileMap::dispose() {
	g_resourceManager->deleteResource(m_tilesetPath);

	for (auto& it : m_animatedTiles) {
		for (auto& tile : it.second) {
			delete tile;
		}
		it.second.clear();
	}
}

const sf::Vector2i& TileMap::getTilesize() const {
	return m_tilesize;
}

