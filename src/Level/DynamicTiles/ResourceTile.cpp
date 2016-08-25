#include "Level/DynamicTiles/ResourceTile.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Screens/LevelScreen.h"

using namespace std;

void ResourceTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_interactComponent = new InteractComponent(g_textProvider->getText("Ore"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToMine");
	m_interactComponent->setOnInteract(std::bind(&ResourceTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void ResourceTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* activeAnimation = new Animation(sf::seconds(10.f));
	activeAnimation->setSpriteSheet(tex);
	activeAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* lootedAnimation = new Animation(sf::seconds(10.f));
	lootedAnimation->setSpriteSheet(tex);
	lootedAnimation->addFrame(sf::IntRect(TILE_SIZE, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Broken, lootedAnimation);

	// initial values
	m_state = GameObjectState::Active;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ResourceTile::setResourceTileData(const ResourceTileData& data) {
	m_objectID = data.objectID;
	m_lootableItems = data.loot.first;
	m_lootableGold = data.loot.second;
}

void ResourceTile::loot() {
	// loot, set blocking for that time
	m_mainChar->executeDefaultFightAnimation(true);
	m_mainChar->lootItems(m_lootableItems);
	if (m_lootableGold > 0) {
		m_mainChar->addGold(m_lootableGold);
	}

	m_interactComponent->setInteractable(false);
	setState(GameObjectState::Broken);
}

void ResourceTile::onRightClick() {
	if (m_mainChar->isDead() || !m_interactComponent->isInteractable()) return;

	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;
	if (inRange) {
		if (m_interactComponent->isInteractable())
			loot();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
	g_inputController->lockAction();
}

std::string ResourceTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_resource.png";
}