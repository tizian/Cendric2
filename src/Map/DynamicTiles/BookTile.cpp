#include "Map/DynamicTiles/BookTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Book, BookTile)

const float BookTile::RANGE = 100.f;

BookTile::BookTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText("Book"), this, m_mainChar);
	interactComponent->setInteractRange(RANGE);
	interactComponent->setInteractText("ToRead");
	interactComponent->setOnInteract(std::bind(&BookTile::startReading, this));
	addComponent(interactComponent);

	g_resourceManager->loadSoundbuffer("res/sound/gui/page_turn.ogg", ResourceType::Map);
}

bool BookTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));

	if (!contains(properties, std::string("id")))
		return false;

	m_bookId = properties.at("id");
	return true;
}

void BookTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void BookTile::onLeftClick() {
	onRightClick();
}

void BookTile::onRightClick() {
	// check if book is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= RANGE) {
		startReading();
	}
	else {
		m_screen->setNegativeTooltip("OutOfRange");
	}
}

void BookTile::startReading() {
	const Item* item = g_resourceManager->getItem(m_bookId);
	if (item == nullptr || !item->getCheck().isDocument) return;

	dynamic_cast<MapScreen*>(m_screen)->setBook(*item);
}

std::string BookTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_book.png";
}

