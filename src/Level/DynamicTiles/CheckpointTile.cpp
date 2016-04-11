#include "Level/DynamicTiles/CheckpointTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Checkpoint, CheckpointTile)

const float CheckpointTile::ACTIVATE_RANGE = 100.f;

void CheckpointTile::init() {
	setSpriteOffset(sf::Vector2f(-15.f, -30.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
}

void CheckpointTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_checkpoint));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * 80, 80, 80));
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activatedAnimation = new Animation(sf::seconds(0.2f));
	activatedAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_checkpoint));
	for (int i = 1; i < 5; i++) {
		activatedAnimation->addFrame(sf::IntRect(i * 80, (skinNr - 1) * 80, 80, 80));
	}
	for (int i = 3; i > 0; i--) {
		activatedAnimation->addFrame(sf::IntRect(i * 80, (skinNr - 1) * 80, 80, 80));
	}
	addAnimation(GameObjectState::Active, activatedAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
		setState(GameObjectState::Active);
	}
}

void CheckpointTile::onRightClick() {
	onLeftClick();
}

void CheckpointTile::onLeftClick() {
	if (m_state == GameObjectState::Active) return;
	g_inputController->lockAction();
	if (dist(m_mainChar->getCenter(), getCenter()) > ACTIVATE_RANGE) {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		return;
	}

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *(m_mainChar->getBoundingBox());
	rec.boundingBox.left = getBoundingBox()->left + getBoundingBox()->width / 2.f - rec.boundingBox.width / 2.f;
	rec.boundingBox.top = getBoundingBox()->top + (getBoundingBox()->height - rec.boundingBox.height);
	if (!m_level->collides(rec)) {
		if (LevelScreen* screen = dynamic_cast<LevelScreen*>(getScreen())) {
			screen->getCharacterCore()->setLevel(sf::Vector2f(rec.boundingBox.left, rec.boundingBox.top), m_level->getID());
			screen->writeToCore();
			screen->setTooltipText("CheckpointReached", COLOR_GOOD, true);
		}
	}
	else {
		g_logger->logError("CheckpointTile", "Setting this checkpoint would stuck our mob, it is not saved.");
		return;
	}

	// deactivate other tiles
	for (auto& it : *m_level->getDynamicTiles()) {
		CheckpointTile* tile = dynamic_cast<CheckpointTile*>(it);
		if (tile == nullptr || tile == this) continue;
		tile->setActive(false);
	}

	// activate this 
	setActive(true);
}

void CheckpointTile::setActive(bool active) {
	setState(active ? GameObjectState::Active : GameObjectState::Idle);
}

