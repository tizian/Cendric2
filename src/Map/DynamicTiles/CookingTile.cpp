#include "Map/DynamicTiles/CookingTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Cooking, CookingTile)

CookingTile::CookingTile(Map* map) : MapDynamicTile(map) {
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(100.f, 100.f)));
}

void CookingTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 
		static_cast<float>(m_tileSize.x), 
		static_cast<float>(m_tileSize.y)));
	setSpriteOffset(sf::Vector2f(0.f, -50.f));
}

void CookingTile::load(int skinNr) {
	m_isCollidable = false;
	int textureHeight = 2 * m_tileSize.y;

	Animation burningAnimation(sf::seconds(0.15f));
	burningAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_cooking));
	for (int i = 0; i < 4; i++) {
		burningAnimation.addFrame(sf::IntRect(m_tileSize.x * i, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	// init tooltip
	m_tooltipText.setString(g_textProvider->getText("Fireplace"));
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

void CookingTile::update(const sf::Time& frameTime) {
	MapDynamicTile::update(frameTime);
	GameObject::updateTime(m_tooltipTime, frameTime);
}

void CookingTile::renderAfterForeground(sf::RenderTarget &renderTarget) {
	MapDynamicTile::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}


void CookingTile::onRightClick() {
	// check if this tile is in range
	sf::Vector2f dist = m_mainCharacter->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 100.f) {
		dynamic_cast<MapScreen*>(m_screen)->setCooking();
	}
	else {
		m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
	}
}

void CookingTile::onInteractKey() {
	onRightClick();
}

void CookingTile::onMouseOver() {
	m_tooltipTime = sf::seconds(1);
}

void CookingTile::setPosition(const sf::Vector2f& pos) {
	m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	MapDynamicTile::setPosition(pos);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

void CookingTile::setDisposed() {
	MapDynamicTile::setDisposed();
	m_lightObject->setDisposed();
}

void CookingTile::setScreen(Screen* screen) {
	MapDynamicTile::setScreen(screen);
	screen->addObject(m_lightObject);
	m_mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
}
