#include "Level/DynamicTiles/LadderTile.h"
#include "Spells/Spell.h"

const int LadderTile::LADDER_STEP = 15;

LadderTile::LadderTile(const LadderTileData& data, LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_size = data.size;
}

void LadderTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F * m_size));
}

void LadderTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	
	sf::Sprite sprite;
	sprite.setTexture(*tex);

	int length = std::max(2, m_size);
	int tilepart = 0;

	sprite.setTextureRect(sf::IntRect((skinNr - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
	m_sprites.push_back(sprite);
	length--;

	while (length > 1) {
		sprite.setTextureRect(sf::IntRect((skinNr - 1) * TILE_SIZE, TILE_SIZE + tilepart * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		m_sprites.push_back(sprite);

		tilepart = (tilepart + 1) % 3;
		length--;
	}

	sprite.setTextureRect(sf::IntRect((skinNr - 1) * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	m_sprites.push_back(sprite);
}

void LadderTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		for (auto& sprite : m_sprites) {
			target.draw(sprite);
		}
		m_isFirstRenderIteration = false;
	}
	else {
		m_isFirstRenderIteration = true;
	}
}

void LadderTile::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	float offset = 0.f;
	for (auto& sprite : m_sprites) {
		sprite.setPosition(getPosition().x, getPosition().y + offset);
		offset += TILE_SIZE;
	}
}

float LadderTile::getClimbingPositionY(GameObject* object) const {
	float ladderBottom = getPosition().y + getBoundingBox()->height;
	float goBottom = object->getPosition().y + object->getBoundingBox()->height;

	int goDiff = static_cast<int>(std::round(ladderBottom - goBottom));
	goDiff = goDiff % LADDER_STEP;
	int sign = 1; // go down

	// check for nearest direction
	if (static_cast<float>(goDiff) > LADDER_STEP / 2.f) {
		goDiff = LADDER_STEP - goDiff;
		sign = -1;
	}

	return goBottom - object->getBoundingBox()->height + sign * goDiff;
}


std::string LadderTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_ladder.png";
}