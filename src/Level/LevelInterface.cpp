#include "Level/LevelInterface.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/WorldScreen.h"
#include "GUI/SlotClone.h"

LevelInterface::LevelInterface(WorldScreen* screen, LevelMainCharacter* character) : WorldInterface(screen),
m_character(character) {
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(character->getAttributes());
	m_healthBar = new HealthBar(character->getAttributes());
	m_quickSlotBar = new QuickSlotBar(this);
	m_spellbook = new Spellbook(m_core, false);
	m_questLog = new QuestLog(m_core);
	m_buffBar = new BuffBar(this);
}

LevelInterface::~LevelInterface() {
	delete m_spellSelection;
	delete m_inventory;
	delete m_characterInfo;
	delete m_healthBar;
	delete m_quickSlotBar;
	delete m_spellbook;
	delete m_questLog;
	delete m_buffBar;
}

void LevelInterface::render(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());
	
	m_buffBar->render(target);
	m_healthBar->render(target);
	m_spellSelection->render(target);
	m_quickSlotBar->render(target);

	WorldInterface::render(target);
}

void LevelInterface::update(const sf::Time& frameTime) {
	if (m_character->isDead()) {
		m_healthBar->update(frameTime);
		return;
	}
		
	WorldInterface::update(frameTime);

	m_healthBar->update(frameTime);
	m_buffBar->update(frameTime);
	m_spellSelection->update(frameTime);
	m_quickSlotBar->update(frameTime);
}

BuffBar& LevelInterface::getBuffBar() {
	return *m_buffBar;
}

void LevelInterface::notifyConsumableDrop(const SlotClone* item) {
	m_quickSlotBar->notifyConsumableDrop(item);
}

void LevelInterface::consumeItem(const std::string& itemID) {
	Item item(itemID);
	if (item.getType() != ItemType::Consumable) return;
	m_character->consumeFood(
		item.getFoodDuration(),
		item.getAttributes());
	getBuffBar().addFoodBuff(
		sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, 50, 50),
		item.getFoodDuration(),
		item.getID(),
		item.getAttributes());

	m_screen->notifyItemChange(item.getID(), -1);
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
 
void LevelInterface::setSpellManager(SpellManager* spellManager) {
	// use this spell manager for the interface bar.
	delete m_spellSelection;
	m_spellSelection = new SpellSelection(spellManager);
}

LevelMainCharacter* LevelInterface::getMainCharacter() const {
	return m_character;
}