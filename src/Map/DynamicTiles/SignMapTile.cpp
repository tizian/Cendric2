#include "Map/DynamicTiles/SignMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "GUI/Hints.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Sign, SignMapTile)

const float SignMapTile::TOOLTIP_TOP = 20.f;
const float SignMapTile::READ_RANGE = 50.f;

SignMapTile::SignMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Sign"), this, m_mainChar);
	m_interactComponent->setInteractRange(READ_RANGE);
	m_interactComponent->setInteractText("ToRead");
	m_interactComponent->setOnInteract(std::bind(&SignMapTile::onInteract, this));
	addComponent(m_interactComponent);

	m_tooltipComponent = new TooltipWindowComponent("", this);
	m_tooltipComponent->setTextAlignment(TextAlignment::Center);
	m_tooltipComponent->setTextOffset(sf::Vector2f(30.f, 10.f));
	addComponent(m_tooltipComponent);
}

void SignMapTile::onInteract() {
	m_tooltipComponent->setCurrentTooltipTime(sf::seconds(3.f));
}

void SignMapTile::update(const sf::Time& frameTime) {
	m_interactComponent->setInteractable(!m_tooltipComponent->isShowingTooltip());

	MapDynamicTile::update(frameTime);
}

bool SignMapTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 20.f));
	setPositionOffset(sf::Vector2f(0.f, 15.f));
	setSpriteOffset(sf::Vector2f(0.f, -15.f));

	if (contains(properties, std::string("hint"))) {
		std::string text = properties.at("hint");
		m_tooltipComponent->setTooltipText(getHintDescription(text));
		m_screen->getCharacterCore()->learnHint(text);
	}
	else if (contains(properties, std::string("text"))) {
		std::string text = properties.at("text");
		m_tooltipComponent->setTooltipText(g_textProvider->getText(text, "sign"));
	}
	else {
		return false;
	}

	if (contains(properties, std::string("name"))) {
		m_interactComponent->setTooltipText(g_textProvider->getText(properties.at("name"), "sign_name"));
	}

	m_isBackground = contains(properties, std::string("bg"));
	m_tooltipComponent->setWindowOffset(sf::Vector2f(0.f, -m_tooltipComponent->getHeight() - TOOLTIP_TOP));

	return true;
}

void SignMapTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

std::string SignMapTile::getSpritePath() const {
	return "res/texture/map_dynamic_tiles/spritesheet_tiles_sign.png";
}

void SignMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	GameObject::onMouseOver();
}

GameObjectType SignMapTile::getConfiguredType() const {
	return m_isBackground ? _DynamicTile : _ForegroundDynamicTile;
}