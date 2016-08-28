#include "Level/DynamicTiles/ResourceTile.h"
#include "Level/LevelMainCharacter.h"
#include "Level/LevelEquipment.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Resource, ResourceTile)

using namespace std;

void ResourceTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_interactComponent = new InteractComponent(g_textProvider->getText(""), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setOnInteract(std::bind(&ResourceTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void ResourceTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_lootTime == sf::Time::Zero) return;

	updateTime(m_lootTime, frameTime);
	if (m_lootTime > sf::Time::Zero) return;

	// add loot and set looted.
	m_mainChar->lootItems(m_lootableItems);
	if (m_lootableGold > 0) {
		m_mainChar->addGold(m_lootableGold);
	}
	setState(GameObjectState::Broken);

	// remove the current tool visualization 
	for (auto go : *m_screen->getObjects(GameObjectType::_Equipment)) {
		LevelEquipment* levelEquipment = dynamic_cast<LevelEquipment*>(go);
		if (levelEquipment != nullptr && levelEquipment->getItemType() == ItemType::Equipment_weapon) {
			levelEquipment->setDisposed();
		}
	}

	// add the visualization for the currentWeapon
	auto& equippedItems = m_screen->getCharacterCore()->getData().equippedItems;
	if (equippedItems.find(ItemType::Equipment_weapon) == equippedItems.end()) return;
	
	const Item* weapon = g_resourceManager->getItem(equippedItems.at(ItemType::Equipment_weapon));
	if (weapon != nullptr && weapon->isEquipmentItem()) {
		LevelEquipment* weaponEquipment = new LevelEquipment(m_mainChar);
		weaponEquipment->load(&weapon->getEquipmentBean(), &weapon->getEquipmentLightBean(), ItemType::Equipment_weapon);
		m_screen->addObject(weaponEquipment);
	}
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

	initializeResource(skinNr);
}

void ResourceTile::initializeResource(int skinNr) {
	// currently, there are only ore tiles
	m_interactComponent->setTooltipText(g_textProvider->getText("Ore"));
	m_interactComponent->setInteractText("ToMine");
	m_noToolMessage = "NeedPickaxe";
	m_toolItemID = "we_pickaxe";

	int oreAmount = rand() % 2 + 1;
	m_lootableItems.insert({ "mi_ironore", oreAmount });
}

void ResourceTile::loot() {
	if (!m_screen->getCharacterCore()->hasItem(m_toolItemID, 1)) {
		m_screen->setTooltipText(m_noToolMessage, COLOR_BAD, true);
		return;
	}
	// loot, set blocking for that time
	m_lootTime = m_mainChar->executeDefaultFightAnimation(true, 3);
	// remove the current weapon visualization 
	for (auto go : *m_screen->getObjects(GameObjectType::_Equipment)) {
		LevelEquipment* levelEquipment = dynamic_cast<LevelEquipment*>(go);
		if (levelEquipment != nullptr && levelEquipment->getItemType() == ItemType::Equipment_weapon) {
			levelEquipment->setDisposed();
		}
	}
	// add the visualization for the tool
	const Item* toolItem = g_resourceManager->getItem(m_toolItemID);
	if (toolItem != nullptr && toolItem->isEquipmentItem()) {
		LevelEquipment* tool = new LevelEquipment(m_mainChar);
		tool->load(&toolItem->getEquipmentBean(), &toolItem->getEquipmentLightBean(), ItemType::Equipment_weapon);
		m_screen->addObject(tool);
	}

	m_interactComponent->setInteractable(false);
}

void ResourceTile::onRightClick() {
	if (m_mainChar->isDead()) return;
	if (!m_interactComponent->isInteractable()) {
		m_screen->setTooltipText("NothingToGain", COLOR_BAD, true);
		return;
	}

	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;
	if (inRange) {
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