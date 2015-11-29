#include "LevelInterface.h"
#include "LevelMainCharacter.h"
#include "Screens/GameScreen.h"

LevelInterface::LevelInterface(GameScreen* screen, LevelMainCharacter* character) : GameInterface(screen),
m_character(character) {
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(character->getAttributes());
	m_healthBar = new HealthBar(character->getAttributes());
	m_quickSlotBar = new QuickSlotBar(this);
	m_spellbook = new Spellbook(m_core, false);
	m_questLog = new QuestLog(m_core);
	m_buffBar = new BuffBar();
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
	GameInterface::render(target);

	m_healthBar->render(target);
	m_buffBar->render(target);
	m_spellSelection->render(target);
	m_quickSlotBar->render(target);
}

void LevelInterface::update(const sf::Time& frameTime) {
	GameInterface::update(frameTime);

	m_healthBar->update();
	m_buffBar->update(frameTime);
	m_spellSelection->update(frameTime);
	m_quickSlotBar->update(frameTime);
}

void LevelInterface::addBuff(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration, SpellID id) {
	m_buffBar->addSlot(type, textureLocation, duration, id);
}

void LevelInterface::removeTypedBuffs(SpellID id) {
	m_buffBar->removeTypedSlots(id);
}

void LevelInterface::notifyConsumableDrop(const InventorySlotClone* item) {
	m_quickSlotBar->notifyConsumableDrop(item);
}

void LevelInterface::consumeItem(const Item& item) {
	m_character->consumeFood(
		item.getBean().foodDuration,
		item.getAttributes());
	addBuff(BuffType::Food,
		sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, 50, 50),
		item.getBean().foodDuration);

	m_screen->notifyItemChange(item.getID(), -1);
	m_quickSlotBar->reload();
}

void LevelInterface::highlightQuickslots(bool highlight) {
	m_quickSlotBar->highlightSlots(highlight);
}

void LevelInterface::reloadInventory(const std::string& changedItemID) {
	GameInterface::reloadInventory(changedItemID);
	m_quickSlotBar->reload();
}

void LevelInterface::setSpellManager(SpellManager* spellManager) {
	// use this spell manager for the interface bar.
	delete m_spellSelection;
	m_spellSelection = new SpellSelection(spellManager);
}

LevelMainCharacter* LevelInterface::getMainCharacter() const {
	return m_character;
}
