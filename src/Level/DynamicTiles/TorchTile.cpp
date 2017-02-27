#include "Level/DynamicTiles/TorchTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Torch, TorchTile)

TorchTile::TorchTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_lightComponent = new LightComponent(LightData(sf::Vector2f(TILE_SIZE_F / 4.f, TILE_SIZE_F / 4.f), sf::Vector2f(140.f, 200.f)), this);
	addComponent(m_lightComponent);
}

void TorchTile::init() {
	setSpriteOffset(sf::Vector2f(-10.f, -TILE_SIZE_F / 2));
	setPositionOffset(sf::Vector2f(12.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F / 2.f, TILE_SIZE_F));
}

void TorchTile::loadAnimation(int skinNr) {
	m_skinNr = skinNr;
	int textureHeight = 2 * TILE_SIZE;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * textureHeight, TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* burningAnimation = new Animation(sf::seconds(0.12f));
	burningAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 9; i++) {
		burningAnimation->addFrame(sf::IntRect(TILE_SIZE * i, skinNr * textureHeight, TILE_SIZE, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void TorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::IceBall:
	case SpellID::Chop:
		if (m_state == GameObjectState::Burning) {
			setState(GameObjectState::Idle);
			m_lightComponent->setVisible(false);
			spell->setDisposed();
		}
		break;
	case SpellID::WindGust:
		if (m_state == GameObjectState::Burning) {
			setState(GameObjectState::Idle);
			m_lightComponent->setVisible(false);
		}
		break;
	case SpellID::FireBall:
		if (m_state == GameObjectState::Idle) {
			setState(GameObjectState::Burning);
			m_lightComponent->setVisible(true);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

int TorchTile::getSkinNr() const {
	return m_skinNr;
}

std::string TorchTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_torch.png";
}
