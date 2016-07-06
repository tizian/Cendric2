#include "Map/DynamicTiles/CookingTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/InteractComponent.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Cooking, CookingTile)

const float CookingTile::RANGE = 100.f;

CookingTile::CookingTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f),
		sf::Vector2f(100.f, 100.f)), this));

	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText("Fireplace"), this, m_mainChar);
	interactComponent->setInteractRange(RANGE);
	interactComponent->setInteractText("ToCook");
	interactComponent->setOnInteract(std::bind(&CookingTile::startCooking, this));
	addComponent(interactComponent);
}

void CookingTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 
		TILE_SIZE_F, 
		TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(0.f, -50.f));
}

void CookingTile::loadAnimation(int skinNr) {
	int textureHeight = 2 * TILE_SIZE;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* burningAnimation = new Animation(sf::seconds(0.15f));
	burningAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		burningAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * textureHeight, TILE_SIZE, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	setState(GameObjectState::Burning);
	playCurrentAnimation(true);
}

void CookingTile::startCooking() {
	dynamic_cast<MapScreen*>(m_screen)->setCooking();
}

void CookingTile::onRightClick() {
	// check if this tile is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= RANGE) {
		startCooking();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}

std::string CookingTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_cooking.png";
}

