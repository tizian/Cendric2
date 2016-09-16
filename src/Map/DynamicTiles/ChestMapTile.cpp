#include "Map/DynamicTiles/ChestMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float ChestMapTile::RANGE = 100.f;

ChestMapTile::ChestMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText("Chest"), this, m_mainChar);
	interactComponent->setInteractRange(RANGE);
	interactComponent->setInteractText("ToLoot");
	interactComponent->setOnInteract(std::bind(&ChestMapTile::loot, this));
	addComponent(interactComponent);
}

void ChestMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void ChestMapTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void ChestMapTile::onLeftClick() {
	onRightClick();
}

void ChestMapTile::onRightClick() {
	// check if chest is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= RANGE) {
		loot();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}

void ChestMapTile::loot() {
	// TODO
}

std::string ChestMapTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_chest.png";
}

