#include "Level/DynamicTiles/CheckpointTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Checkpoint, CheckpointTile)

void CheckpointTile::init() {
	setSpriteOffset(sf::Vector2f(-15.f, -30.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
}

void CheckpointTile::load(int skinNr) {
	m_isCollidable = false;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_checkpoint));
	idleAnimation.addFrame(sf::IntRect(0, (skinNr - 1) * 80, 80, 80));
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation activatedAnimation(sf::seconds(0.2f));
	activatedAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_checkpoint));
	for (int i = 1; i < 5; i++) {
		activatedAnimation.addFrame(sf::IntRect(i * 80, (skinNr - 1) * 80, 80, 80));
	}
	for (int i = 3; i > 0; i--) {
		activatedAnimation.addFrame(sf::IntRect(i * 80, (skinNr - 1) * 80, 80, 80));
	}
	addAnimation(GameObjectState::Active, activatedAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void CheckpointTile::onHit(LevelMovableGameObject* mob) {
	if (m_state == GameObjectState::Active) return;
	if (LevelMainCharacter* character = dynamic_cast<LevelMainCharacter*>(mob)) {
		m_state = GameObjectState::Active;
		setCurrentAnimation(getAnimation(m_state), false);

		sf::FloatRect bb = *(character->getBoundingBox());
		bb.left = getBoundingBox()->left + getBoundingBox()->width / 2.f - bb.width / 2.f;
		bb.top = getBoundingBox()->top + (getBoundingBox()->height - bb.height);
		if (!m_level->collides(bb)) {
			if (LevelScreen* screen = dynamic_cast<LevelScreen*>(getScreen())) {
				screen->getCharacterCore()->setLevel(sf::Vector2f(bb.left, bb.top), m_level->getID());
				screen->writeToCore();
				screen->setTooltipText(g_textProvider->getText("CheckpointReached"), sf::Color::Green, true);
			}
		}
		else {
			g_logger->logError("CheckpointTile", "Setting this checkpoint would stuck our mob, it is not saved.");
		}
	}
}

void CheckpointTile::onHit(Spell* spell) {
	// nop yet, maybe there is some spell that alters checkpoints?
}