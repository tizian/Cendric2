#include "Map/DynamicTiles/BookTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/TooltipComponent.h"

BookTile::BookTile(const BookData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;
	addComponent(new TooltipComponent("Book", this));
}

void BookTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));
}

void BookTile::loadAnimation(int skinNr) {

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_book));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void BookTile::onLeftClick() {
	onRightClick();
}

void BookTile::onRightClick() {
	// check if npc is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 100.f) {
		MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
		mapScreen->setBook(&m_data);
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}
