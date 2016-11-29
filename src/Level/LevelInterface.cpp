#include "Level/LevelInterface.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/WorldScreen.h"
#include "GUI/SlotClone.h"

LevelInterface::LevelInterface(WorldScreen* screen, LevelMainCharacter* character) : WorldInterface(screen),
m_character(character) {
	m_sidebar = new GUITabBar(this);
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(screen, character->getAttributes());
	m_quickSlotBar = new QuickSlotBar(this);
	m_spellbook = new Spellbook(m_core, false);
	m_questLog = new QuestLog(m_core);
	m_buffBar = new BuffBar(this);
	m_mainCharHealthBar = new HealthBar(character->getAttributes(), HealthBarStyle::MainCharacter);

	const Level* level = dynamic_cast<const Level*>(screen->getWorld());
	if (level->getWorldData()->bossLevelData.isBossLevel) {
		// Assume first enemy in level is boss
		m_enemyHealthBar = new HealthBar(nullptr, HealthBarStyle::Boss);
	}
	else {
		m_enemyHealthBar = new HealthBar(nullptr, HealthBarStyle::Enemy);
	}
	m_enemyHealthBar->setVisible(false);
}

LevelInterface::~LevelInterface() {
	delete m_sidebar;
	delete m_spellSelection;
	delete m_inventory;
	delete m_characterInfo;
	delete m_mainCharHealthBar;
	delete m_enemyHealthBar;
	delete m_quickSlotBar;
	delete m_spellbook;
	delete m_questLog;
	delete m_buffBar;
}

void LevelInterface::render(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());
	
	m_buffBar->render(target);

	m_mainCharHealthBar->render(target);
	m_enemyHealthBar->render(target);
	
	m_spellSelection->render(target);
	m_quickSlotBar->render(target);

	WorldInterface::render(target);
}

void LevelInterface::renderAfterForeground(sf::RenderTarget& target) {
	WorldInterface::renderAfterForeground(target);
	
	m_quickSlotBar->renderAfterForeground(target);
	m_spellSelection->renderAfterForeground(target);
}

void LevelInterface::update(const sf::Time& frameTime) {
	if (m_character->isDead()) {
		m_mainCharHealthBar->update(frameTime);
		return;
	}
		
	WorldInterface::update(frameTime);

	m_mainCharHealthBar->update(frameTime);
	m_enemyHealthBar->update(frameTime);

	m_buffBar->update(frameTime);
	m_spellSelection->update(frameTime);
	m_quickSlotBar->update(frameTime);
}

void LevelInterface::setEnemyForHealthBar(const Enemy* enemy) {
	if (enemy == nullptr) {
		m_enemyHealthBar->setVisible(false);
		m_enemyHealthBar->setAttributes(nullptr);
		return;
	}
	m_enemyHealthBar->setName(g_textProvider->getText(EnumNames::getEnemyName(enemy->getEnemyID()), "enemy"));
	m_enemyHealthBar->setAttributes(enemy->getAttributes());
	m_enemyHealthBar->setVisible(true);
}

BuffBar& LevelInterface::getBuffBar() {
	return *m_buffBar;
}

void LevelInterface::notifyConsumableDrop(const SlotClone* item) {
	m_quickSlotBar->notifyConsumableDrop(item);
}

void LevelInterface::equipConsumable(const std::string& itemID) {
	m_quickSlotBar->equipConsumable(itemID);
}

void LevelInterface::clearConsumedFood() {
	m_consumedFood.clear();
}

void LevelInterface::restoreConsumedFood() {
	for (auto& it : m_consumedFood) {
		getScreen()->getCharacterCore()->notifyItemChange(it.first, it.second);
	}
	m_consumedFood.clear();
}

void LevelInterface::consumeItem(const std::string& itemID) {
	Item* item = g_resourceManager->getItem(itemID);
	if (item == nullptr || item->getType() != ItemType::Consumable) return;
	if (m_character->isEating()) return;
	m_character->consumeFood(
		item->getFoodDuration(),
		item->getAttributes());
	getBuffBar().addFoodBuff(
		sf::IntRect(item->getIconTextureLocation().x, item->getIconTextureLocation().y, 50, 50),
		item->getFoodDuration(),
		item->getID(),
		item->getAttributes());

	if (!contains(m_consumedFood, item->getID())) {
		m_consumedFood.insert({ item->getID(), 0 });
	}
	m_consumedFood[item->getID()] += 1;

	m_screen->notifyItemChange(item->getID(), -1);
	m_quickSlotBar->reload();
}

void LevelInterface::highlightQuickslots(bool highlight) {
	m_quickSlotBar->highlightSlots(highlight);
}

void LevelInterface::reloadInventory(const std::string& changedItemID) {
	WorldInterface::reloadInventory(changedItemID);
	m_quickSlotBar->reload();
}

void LevelInterface::notifyCharacterInfo() {
	m_characterInfo->notifyChange();
}

bool LevelInterface::isEnemyHealthBarDisplayed() {
	return m_enemyHealthBar->getAttributes() != nullptr;
}
 
void LevelInterface::setSpellManager(SpellManager* spellManager) {
	// use this spell manager for the interface bar.
	delete m_spellSelection;
	m_spellSelection = new SpellSelection(spellManager);
}

void LevelInterface::setPermanentCore(CharacterCore* permanentCore) {
	m_permanentCore = permanentCore;
}

LevelMainCharacter* LevelInterface::getMainCharacter() const {
	return m_character;
}

CharacterCore* LevelInterface::getPermanentCore() const {
	return m_permanentCore;
}