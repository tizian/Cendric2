#include "Map/DynamicTiles/DoorMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float DoorMapTile::OPEN_RANGE = 50.f;

DoorMapTile::DoorMapTile(const DoorData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;

	m_interactComponent = new InteractComponent(g_textProvider->getText("Door"), this, m_mainChar);
	m_interactComponent->setInteractRange(OPEN_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&DoorMapTile::onRightClick, this));
	addComponent(m_interactComponent);

	reloadConditions();
}

void DoorMapTile::open() {
	m_open = true;
	m_isCollidable = false;
	setState(GameObjectState::Open);
	m_interactComponent->setInteractable(false);
}

void DoorMapTile::close() {
	m_open = false;
	m_isCollidable = true;
	setState(GameObjectState::Closed);
	m_interactComponent->setInteractable(true);
}

void DoorMapTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Closed) {
		m_interactComponent->setInteractable(true);
	}
	
	if (!m_conditionsFulfilled) {
		m_interactComponent->setInteractable(false);
	}

	if (m_reloadNeeded) {
		reloadConditions();
		m_reloadNeeded = false;
	}

	MapDynamicTile::update(frameTime);
}

void DoorMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setPositionOffset(sf::Vector2f(0.f, 0.f));
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void DoorMapTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* openAnimation = new Animation();
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Open, openAnimation);

	Animation* closedAnimation = new Animation();
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(TILE_SIZE, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Closed, closedAnimation);

	playCurrentAnimation(false);
}

std::string DoorMapTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_doors.png";
}

void DoorMapTile::setPosition(const sf::Vector2f& pos) {
	MapDynamicTile::setPosition(pos);
}

void DoorMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_interactComponent->setInteractable(false);
}

void DoorMapTile::onRightClick() {
	if (m_open || !m_conditionsFulfilled) return;

	// check if the door is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= OPEN_RANGE;

	if (!inRange) {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
	else if (!m_data.keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_data.keyItemID, 1)) {
		open();

		std::string tooltipText = g_textProvider->getText("Used");
		tooltipText.append(g_textProvider->getText(m_data.keyItemID, "item"));
		m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);

		g_inputController->lockAction();
	}
	else {
		m_screen->setTooltipText("IsLocked", COLOR_BAD, true);
	}
}

void DoorMapTile::reloadConditions() {
	CharacterCore* core = m_screen->getCharacterCore();

	bool conditionsFulfilled = true;
	for (auto& condition : m_data.conditions) {
		if (condition.negative) {
			conditionsFulfilled = conditionsFulfilled && !core->isConditionFulfilled(condition.type, condition.name);
		}
		else if (!condition.negative) {
			conditionsFulfilled = conditionsFulfilled && core->isConditionFulfilled(condition.type, condition.name);
		}
	}

	m_conditionsFulfilled = conditionsFulfilled;

	if (m_conditionsFulfilled && m_data.keyItemID.empty()) {
		open();
	}
	else {
		close();
	}
}

void DoorMapTile::notifyReloadNeeded() {
	m_reloadNeeded = true;
}