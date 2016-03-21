#include "Level/DynamicTiles/LeverTile.h"
#include "Spells/Spell.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/TooltipComponent.h"

LeverTile::LeverTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	addComponent(new TooltipComponent(g_textProvider->getText("Lever"), this));
}

// this tile is special and is not registered
void LeverTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void LeverTile::loadAnimation(int skinNr) {

	Animation* onAnimation = new Animation(sf::seconds(10.0f));
	onAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_lever));
	onAnimation->addFrame(sf::IntRect(
		BORDER,
		BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
		TILE_SIZE,
		TILE_SIZE));

	addAnimation(GameObjectState::On, onAnimation);

	Animation* offAnimation = new Animation(sf::seconds(10.0f));
	offAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_lever));
	offAnimation->addFrame(sf::IntRect(
		BORDER + (2 * BORDER + TILE_SIZE),
		BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
		TILE_SIZE,
		TILE_SIZE));

	addAnimation(GameObjectState::Off, offAnimation);

	// initial values
	setState(GameObjectState::Off);
	playCurrentAnimation(false);
}

void LeverTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis:
		switchLever();
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void LeverTile::onRightClick() {
	if (m_mainChar->isDead()) return;
	// check if lever is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= ACTIVATE_RANGE) {
		switchLever();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
	g_inputController->lockAction();
}

void LeverTile::setDependantTiles(const std::vector<SwitchableTile*>& dependentTiles) {
	m_dependentTiles = dependentTiles;
}

void LeverTile::switchLever() {
	WorldCollisionQueryRecord rec;
	for (auto& tile : m_dependentTiles) {
		if (tile->getGameObjectState() == GameObjectState::On) continue;
		rec.boundingBox = *tile->getBoundingBox();
		if (m_level->collidesWithMobs(rec) || m_level->collidesWithMovableTiles(rec)) {
			g_logger->logInfo("LeverTile::switchLever", "Cannot switch the lever as it would stuck a MOB or a movable tile!");
			m_screen->setTooltipText("LeverStuck", COLOR_BAD, true);
			return;
		}
	}

	(m_state == GameObjectState::On) ? 
		setState(GameObjectState::Off) : 
		setState(GameObjectState::On);

	g_resourceManager->playSound(m_sound, ResourceID::Sound_tile_lever);

	for (auto& tile : m_dependentTiles) {
		tile->switchTile();
	}
}