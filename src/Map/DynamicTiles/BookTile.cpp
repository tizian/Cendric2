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
	// check if book is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= RANGE) {
		startReading();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}

void BookTile::startReading() {
	dynamic_cast<MapScreen*>(m_screen)->setBook(&m_data);
}
