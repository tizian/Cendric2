#include "Map/DynamicTiles/SignMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GUI/Hints.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Sign, SignMapTile)

const float SignMapTile::TOOLTIP_TOP = 20.f;
const float SignMapTile::READ_RANGE = 50.f;
const sf::Time SignMapTile::TOOLTIP_WINDOW_TIME = sf::seconds(3.f);

SignMapTile::SignMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_tooltipWindow.setTextOffset(sf::Vector2f(30.f, 10.f));
	m_tooltipWindow.setTextAlignment(TextAlignment::Center);
	m_showTooltip = false;

	m_interactComponent = new InteractComponent(g_textProvider->getText("Sign"), this, m_mainChar);
	m_interactComponent->setInteractRange(READ_RANGE);
	m_interactComponent->setInteractText("ToRead");
	m_interactComponent->setOnInteract(std::bind(&SignMapTile::onInteract, this));
	addComponent(m_interactComponent);
}

void SignMapTile::onInteract() {
	m_tooltipWindowTime = TOOLTIP_WINDOW_TIME;
	m_tooltipWindow.setPosition(sf::Vector2f(
		getPosition().x,
		getPosition().y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
}

void SignMapTile::update(const sf::Time& frameTime) {
	m_showTooltip = m_tooltipWindowTime > sf::Time::Zero;
	m_interactComponent->setInteractable(!m_showTooltip);
	updateTime(m_tooltipWindowTime, frameTime);

	MapDynamicTile::update(frameTime);
}

bool SignMapTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 20.f));
	setPositionOffset(sf::Vector2f(0.f, 15.f));
	setSpriteOffset(sf::Vector2f(0.f, -15.f));

	if (contains(properties, std::string("hint"))) {
		std::string text = properties.at("hint");
		m_tooltipWindow.setText(getHintDescription(text));
		m_screen->getCharacterCore()->learnHint(text);
	}
	else if (contains(properties, std::string("text"))) {
		std::string text = properties.at("text");
		m_tooltipWindow.setText(g_textProvider->getText(text, "sign"));
	}
	else {
		return false;
	}

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
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_sign.png";
}

void SignMapTile::setPosition(const sf::Vector2f& pos) {
	MapDynamicTile::setPosition(pos);
	m_tooltipWindow.setPosition(sf::Vector2f(pos.x, pos.y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
}

void SignMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_showTooltip = true;
	m_interactComponent->setInteractable(false);
	// update tooltip position
	sf::Vector2f mouse = g_inputController->getMousePosition();
	sf::Vector2f size = m_tooltipWindow.getSize();
	m_tooltipWindow.setPosition(sf::Vector2f(mouse.x - 0.5f * size.x, mouse.y - size.y - TOOLTIP_TOP));
}

void SignMapTile::renderAfterForeground(sf::RenderTarget& renderTarget) {
	MapDynamicTile::renderAfterForeground(renderTarget);
	if (m_showTooltip) {
		m_tooltipWindow.render(renderTarget);
	}
}

GameObjectType SignMapTile::getConfiguredType() const {
	return GameObjectType::_ForegroundDynamicTile;
}