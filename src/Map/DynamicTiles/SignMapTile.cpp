#include "Map/DynamicTiles/SignMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"

const float TOOLTIP_TOP = 20.f;

SignMapTile::SignMapTile(const SignData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;
	m_tooltipWindow.setText(data.title);
	m_tooltipWindow.setTextOffset(sf::Vector2f(30.f, 10.f));
	m_showTooltip = false;
}

void SignMapTile::update(const sf::Time& frameTime) {
	m_showTooltip = false;
	MapDynamicTile::update(frameTime);
}

void SignMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 20.f));
	setPositionOffset(sf::Vector2f(0.f, 15.f));
	setSpriteOffset(sf::Vector2f(0.f, -15.f));
}

void SignMapTile::loadAnimation(int skinNr) {
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_sign_map));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SignMapTile::setPosition(const sf::Vector2f& pos) {
	MapDynamicTile::setPosition(pos);
	m_tooltipWindow.setPosition(sf::Vector2f(pos.x, pos.y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
}

void SignMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_showTooltip = true;
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