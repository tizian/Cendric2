#include "Map/DynamicTiles/ChestMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float ChestMapTile::PICKUP_RANGE = 100.f;

ChestMapTile::ChestMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Chest"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&ChestMapTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void ChestMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
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

void ChestMapTile::setChestData(const ChestTileData& data) {
	m_data = data;
	if (m_data.chestStrength < 0 || m_data.chestStrength > 5) {
		m_data.chestStrength = 0;
	}

	if (data.isOpen) {
		unlock();
	}
	if (!data.tooltipText.empty()) {
		m_interactComponent->setTooltipText(g_textProvider->getText(data.tooltipText, "chest"));
	}
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
	else if (m_data.chestStrength == 0 && m_state == GameObjectState::Locked) {
		if (inRange) {
			unlock();
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else if (!m_data.keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_data.keyItemID, 1)) {
		if (inRange) {
			unlock();
			std::string tooltipText = g_textProvider->getText("Used");
			tooltipText.append(g_textProvider->getText(m_data.keyItemID, "item"));
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

	if (m_data.isStoredItems) {
		auto items = screen->getCharacterCore()->retrieveStoredItems();
		for (auto& item : items) {
			screen->notifyItemChange(item.first, item.second);
		}
		screen->notifyItemChange("gold", screen->getCharacterCore()->retrieveStoredGold());
		
	}
	else {
		for (auto& item : m_data.loot.first) {
			screen->notifyItemChange(item.first, item.second);
		}
		screen->notifyItemChange("gold", m_data.loot.second);
	}
	
	if (!m_data.conditionProgress.type.empty() && !m_data.conditionProgress.name.empty()) {
		screen->notifyConditionAdded(m_data.conditionProgress);
	}

	m_interactComponent->setInteractable(false);

	if (!m_data.isPermanent) {
		setDisposed();
	}
}

void ChestMapTile::unlock() {
	m_interactComponent->setInteractText("ToPickup");
	setState(GameObjectState::Unlocked);
}

std::string ChestMapTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_chest.png";
}

