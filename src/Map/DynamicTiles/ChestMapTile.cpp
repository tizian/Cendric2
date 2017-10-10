#include "Map/DynamicTiles/ChestMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Chest, ChestMapTile)

const float ChestMapTile::PICKUP_RANGE = 100.f;

ChestMapTile::ChestMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Chest"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&ChestMapTile::onRightClick, this));
	addComponent(m_interactComponent);
}

bool ChestMapTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	ChestTile::init(properties);

	if (m_isOpen) {
		unlock();
	}
	if (!m_tooltipText.empty()) {
		m_interactComponent->setTooltipText(g_textProvider->getText(m_tooltipText, "chest"));
	}
	if (m_lightData.radius.x > 0.f) {
		addComponent(new LightComponent(m_lightData, this));
	}

	return true;
}

void ChestMapTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* closedAnimation = new Animation(sf::seconds(10.f));
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Locked, closedAnimation);

	Animation* openAnimation = new Animation(sf::seconds(10.f));
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(TILE_SIZE, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Unlocked, openAnimation);

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ChestMapTile::onLeftClick() {
	onRightClick();
}

void ChestMapTile::onRightClick() {
	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;

	if (m_state == GameObjectState::Unlocked) {
		if (inRange) {
			loot();
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else if (m_chestStrength == 0 && m_state == GameObjectState::Locked) {
		if (inRange) {
			unlock();
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else if (!m_keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_keyItemID, 1)) {
		if (inRange) {
			unlock();
			std::string tooltipText = g_textProvider->getText("Used");
			tooltipText.append(g_textProvider->getText(m_keyItemID, "item"));
			m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else {
		m_screen->setNegativeTooltip("IsLocked");
	}
}

void ChestMapTile::loot() {
	MapScreen* screen = dynamic_cast<MapScreen*>(m_screen);

	if (m_isStoredItems) {
		auto items = screen->getCharacterCore()->retrieveStoredItems();
		for (auto& item : items) {
			screen->notifyItemChange(item.first, item.second);
		}
		screen->notifyItemChange("gold", screen->getCharacterCore()->retrieveStoredGold());
		
	}
	else {
		for (auto& item : m_lootableItems) {
			screen->notifyItemChange(item.first, item.second);
		}
		screen->notifyItemChange("gold", m_lootableGold);
	}

	m_interactComponent->setInteractable(false);

	if (!m_isPermanent) {
		setDisposed();
	}
}

void ChestMapTile::unlock() {
	m_interactComponent->setInteractText("ToPickup");
	setState(GameObjectState::Unlocked);
}

std::string ChestMapTile::getSpritePath() const {
	return "res/texture/map_dynamic_tiles/spritesheet_tiles_chest.png";
}

