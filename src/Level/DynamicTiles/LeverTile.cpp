#include "Level/DynamicTiles/LeverTile.h"
#include "Spell.h"
#include "Level/LevelMainCharacter.h"

// this tile is special and is not registered
void LeverTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void LeverTile::load(int skinNr) {

	Animation onAnimation(sf::seconds(10.0f));
	onAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_lever));
	onAnimation.addFrame(sf::IntRect(
		BORDER,
		BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y),
		m_tileSize.x,
		m_tileSize.y));

	addAnimation(GameObjectState::On, onAnimation);

	Animation offAnimation(sf::seconds(10.0f));
	offAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_lever));
	offAnimation.addFrame(sf::IntRect(
		BORDER + (2 * BORDER + m_tileSize.x),
		BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y),
		m_tileSize.x,
		m_tileSize.y));

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
	// check if lever is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= ACTIVATE_RANGE) {
		switchLever();
	}
	else {
		m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
	}
	g_inputController->lockAction();
}

void LeverTile::onInteractKey() {
	// it is made impossible for a game object to have onInteractKey and onRightClick called in the same game loop
	onRightClick();
}

void LeverTile::onMouseOver() {
	m_animatedSprite.setColor(sf::Color::Red);
}

void LeverTile::render(sf::RenderTarget &renderTarget) {
	AnimatedGameObject::render(renderTarget);
	m_animatedSprite.setColor(sf::Color::White);
}

void LeverTile::setDependantTiles(const std::vector<SwitchableTile*>& dependentTiles) {
	m_dependentTiles = dependentTiles;
}

void LeverTile::switchLever() {
	for (auto& tile : m_dependentTiles) {
		if (tile->getGameObjectState() == GameObjectState::On) continue;
		if (m_level->collidesWithMobs(*tile->getBoundingBox())) {
			g_logger->logInfo("LeverTile::switchLever", "Cannot switch the lever as it would stuck a MOB!");
			m_screen->setTooltipText(g_textProvider->getText("LeverStuck"), sf::Color::Red, true);
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