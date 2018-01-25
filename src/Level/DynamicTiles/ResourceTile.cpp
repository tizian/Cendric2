#include "Level/DynamicTiles/ResourceTile.h"
#include "Level/LevelMainCharacter.h"
#include "Level/LevelEquipment.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Screens/LevelScreen.h"
#include "World/Item.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Resource, ResourceTile)

const std::string ResourceTile::PICK_SOUND_PATH = "res/sound/weapon/pickaxe.ogg";

bool ResourceTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_interactComponent = new InteractComponent(g_textProvider->getText(""), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setOnInteract(std::bind(&ResourceTile::onRightClick, this));
	addComponent(m_interactComponent);
	return true;
}

void ResourceTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	updatePicking(frameTime);
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
	for (auto go : *m_screen->getObjects(_Equipment)) {
		LevelEquipment* levelEquipment = dynamic_cast<LevelEquipment*>(go);
		if (levelEquipment != nullptr && levelEquipment->getItemType() == ItemType::Equipment_weapon) {
			levelEquipment->setDisposed();
		}
	}

	// add the visualization for the currentWeapon
	auto& equippedItems = m_screen->getCharacterCore()->getData().equippedItems;
	if (equippedItems.find(ItemType::Equipment_weapon) == equippedItems.end()) return;
	
	const Item* weapon = g_resourceManager->getItem(equippedItems.at(ItemType::Equipment_weapon));
	if (weapon != nullptr && weapon->getCheck().isEquipment) {
		LevelEquipment* weaponEquipment = new LevelEquipment(m_mainChar);
		weaponEquipment->load(weapon->getBean<ItemEquipmentBean>(), ItemType::Equipment_weapon);
		weaponEquipment->loadComponents(weapon->getBean<ItemEquipmentLightBean>(), weapon->getBean<ItemEquipmentParticleBean>());
		m_screen->addObject(weaponEquipment);
	}
}

void ResourceTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* activeAnimation = new Animation(sf::seconds(10.f));
	activeAnimation->setSpriteSheet(tex);
	activeAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* lootedAnimation = new Animation(sf::seconds(10.f));
	lootedAnimation->setSpriteSheet(tex);
	lootedAnimation->addFrame(sf::IntRect(TILE_SIZE, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Broken, lootedAnimation);

	// initial values
	m_state = GameObjectState::Active;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);

	if (skinNr == 2|| skinNr == 3) {
		// shift them a bit
		setSpriteOffset(sf::Vector2f((skinNr == 2) ? -10.f : 10.f, 0.f));
	}

	initializeResource(skinNr);
}

void ResourceTile::initializeResource(int skinNr) {
	// currently, there are only ore tiles
	m_interactComponent->setTooltipText(g_textProvider->getText("Ore"));
	m_interactComponent->setInteractText("ToMine");
	m_noToolMessage = "NeedPickaxe";
	m_toolItemID = "we_pickaxe";

	int oreAmount = rand() % 2 + 1;
	bool hasShinyStone = rand() % 100 > 90;
	m_lootableItems.insert({ "mi_ironore", oreAmount });
	if (hasShinyStone) {
		m_lootableItems.insert({ "mi_shinystone", 1 });
	}
}

void ResourceTile::loot() {
	if (!m_screen->getCharacterCore()->hasItem(m_toolItemID, 1)) {
		m_screen->setNegativeTooltip(m_noToolMessage);
		return;
	}
	// loot, set blocking for that time
	m_pickCount = 3;
	m_pickCooldown = sf::Time::Zero;
	m_lootTime = m_mainChar->executeDefaultFightAnimation(true, m_pickCount);
	// remove the current weapon visualization 
	for (auto go : *m_screen->getObjects(_Equipment)) {
		LevelEquipment* levelEquipment = dynamic_cast<LevelEquipment*>(go);
		if (levelEquipment != nullptr && levelEquipment->getItemType() == ItemType::Equipment_weapon) {
			levelEquipment->setDisposed();
		}
	}
	// add the visualization for the tool
	const Item* toolItem = g_resourceManager->getItem(m_toolItemID);
	if (toolItem != nullptr && toolItem->getCheck().isEquipment) {
		LevelEquipment* tool = new LevelEquipment(m_mainChar);
		tool->load(toolItem->getBean<ItemEquipmentBean>(), ItemType::Equipment_weapon);
		tool->loadComponents(toolItem->getBean<ItemEquipmentLightBean>(), toolItem->getBean<ItemEquipmentParticleBean>());
		m_screen->addObject(tool);
	}

	m_interactComponent->setInteractable(false);
}

void ResourceTile::onRightClick() {
	if (m_mainChar->isDead()) return;
	if (!m_interactComponent->isInteractable()) {
		m_screen->setNegativeTooltip("NothingToGain");
		return;
	}

	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;
	if (inRange) {
		loot();
	}
	else {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	g_inputController->lockAction();
}

std::string ResourceTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_resource.png";
}

void ResourceTile::updatePicking(const sf::Time& frameTime) {
	if (m_pickCount == 0) return;
	updateTime(m_pickCooldown, frameTime);
	if (m_pickCooldown > sf::Time::Zero) {
		return;
	}
	m_pickCooldown = sf::milliseconds(5 * 70);
	g_resourceManager->playSound(PICK_SOUND_PATH, false);
	m_pickCount--;
}