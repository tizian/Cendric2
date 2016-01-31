#include "Level/DynamicTiles/TorchTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Torch, TorchTile)

TorchTile::TorchTile(Level* level) : LevelDynamicTile(level) {
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(140.f, 200.f)));
}

void TorchTile::init() {
	setSpriteOffset(sf::Vector2f(-10.f, -TILE_SIZE_F / 2));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F / 2.f, TILE_SIZE_F));
}

void TorchTile::loadAnimation(int skinNr) {
	int textureHeight = 2 * TILE_SIZE;

	Animation idleAnimation(sf::seconds(10.f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	idleAnimation.addFrame(sf::IntRect(0, 0, TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation burningAnimation(sf::seconds(0.12f));
	burningAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	for (int i = 1; i < 9; i++) {
		burningAnimation.addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * textureHeight, TILE_SIZE, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void TorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Chop:
		spell->setDisposed();
		setDisposed();
		break;
	case SpellID::IceBall:
		if (m_state == GameObjectState::Burning) {
			m_state = GameObjectState::Idle;
			m_lightObject->setVisible(false);
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	case SpellID::FireBall:
		if (m_state == GameObjectState::Idle) {
			m_state = GameObjectState::Burning;
			m_lightObject->setVisible(true);
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

void TorchTile::setPosition(const sf::Vector2f& pos) {
	m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	LevelDynamicTile::setPosition(pos);
}

void TorchTile::setDisposed() {
	LevelDynamicTile::setDisposed();
	m_lightObject->setDisposed();
}

void TorchTile::setScreen(Screen* screen) {
	LevelDynamicTile::setScreen(screen);
	screen->addObject(m_lightObject);
}
