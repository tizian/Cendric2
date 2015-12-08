#include "Map/DynamicTiles/CookingTile.h"
#include "Map/Map.h"

CookingTile::CookingTile(Map* map) : MapDynamicTile(map) {
	m_lightObject = new LightObject(LightBean(sf::Vector2f(), sf::Vector2f(100.f, 100.f)));
}

void CookingTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 
		static_cast<float>(m_tileSize.x), 
		static_cast<float>(m_tileSize.y) * 2.f));
}

void CookingTile::load(int skinNr) {
	m_isCollidable = false;
	int textureHeight = 2 * m_tileSize.y;

	Animation burningAnimation(sf::seconds(0.15f));
	burningAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_cooking));
	for (int i = 1; i < 5; i++) {
		burningAnimation.addFrame(sf::IntRect(m_tileSize.x * i, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void CookingTile::setPosition(const sf::Vector2f& pos) {
	m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	MapDynamicTile::setPosition(pos);
}

void CookingTile::setDisposed() {
	MapDynamicTile::setDisposed();
	m_lightObject->setDisposed();
}

void CookingTile::setScreen(Screen* screen) {
	MapDynamicTile::setScreen(screen);
	screen->addObject(m_lightObject);
}
