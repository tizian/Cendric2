#include "Map/DynamicTiles/BookTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float BookTile::RANGE = 100.f;

BookTile::BookTile(const BookData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;
	
	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText("Book"), this, m_mainChar);
	interactComponent->setInteractRange(RANGE);
	interactComponent->setInteractText("ToRead");
	interactComponent->setOnInteract(std::bind(&BookTile::startReading, this));
	addComponent(interactComponent);

	g_resourceManager->loadSoundbuffer("res/sound/gui/page_turn.ogg", ResourceType::Map);
}

void BookTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));
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
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}

void BookTile::startReading() {
	const Item* item = g_resourceManager->getItem(m_data.id);
	if (item == nullptr || !item->isDocument()) return;

	dynamic_cast<MapScreen*>(m_screen)->setBook(*item);
}

std::string BookTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_book.png";
}

