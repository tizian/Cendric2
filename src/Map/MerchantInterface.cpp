#include "Map/MerchantInterface.h"

MerchantInterface::MerchantInterface(WorldScreen* screen, std::string merchantID) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
	m_merchantID = merchantID;
	m_data = m_core->getMerchantData(merchantID);
	m_merchantWindow = new MerchantWindow(this);
	m_inventory = m_screen->getInventory();
	m_inventory->startTrading(this);
}

MerchantInterface::~MerchantInterface() {
	if (m_screen->getInventory() != nullptr) {
		m_screen->getInventory()->stopTrading();
	}
	delete m_merchantWindow;
}

void MerchantInterface::completeTrade() {
	if (m_screen->getInventory() != nullptr) {
		m_screen->getInventory()->stopTrading();
	}
	m_core->setMerchantData(m_merchantID, m_data.wares);
	m_isCancelled = true;
}

void MerchantInterface::sellItem(const Item* item) {
	if (item == nullptr) return;
	if (item->getValue() < 0) {
		m_screen->setNegativeTooltip("Unsalable");
		return;
	}
	std::string id = item->getID();
	m_screen->notifyItemChange("gold", (int)std::ceil(m_data.receiver_multiplier * item->getValue()));
	if (!contains(m_data.wares, id)) {
		m_data.wares.insert({ id, 1 });
	}
	else {
		int amount = m_data.wares.at(id);
		amount++;
		m_data.wares[id] = amount;
	}
	m_merchantWindow->notifyChange(id);
	m_screen->notifyItemChange(id, -1);
}

void MerchantInterface::buyItem(const Item* item) {
	if (item == nullptr) return;
	if (m_core->getData().gold < (int)std::ceil(m_data.multiplier * item->getValue())) {
		m_screen->setNegativeTooltip("NotEnoughGold");
		return;
	}
	if (!isReputationReached(item)) {
		m_screen->setNegativeTooltip("NotEnoughReputation");
		return;
	}
	std::string id = item->getID();
	m_screen->notifyItemChange("gold", -(int)std::ceil(m_data.multiplier * item->getValue()));
	m_screen->notifyItemChange(id, 1);
	if (contains(m_data.wares, id)) {
		int amount = m_data.wares.at(id);
		amount--;
		if (amount <= 0) {
			m_data.wares.erase(m_data.wares.find(id));
		}
		else {
			m_data.wares[id] = amount;
		}
	}
	m_merchantWindow->notifyChange(id);
}

bool MerchantInterface::isCancelled() {
	return m_isCancelled;
}

void MerchantInterface::render(sf::RenderTarget& target) {
	m_merchantWindow->render(target);
}

void MerchantInterface::renderAfterForeground(sf::RenderTarget& target) {
	m_merchantWindow->renderAfterForeground(target);
}

void MerchantInterface::update(const sf::Time& frameTime) {
	m_inventory->update(frameTime);
	m_merchantWindow->update(frameTime);
}

CharacterCore* MerchantInterface::getCore() const {
	return m_core;
}

const MerchantData& MerchantInterface::getMerchantData() const {
	return m_data;
}

const std::string& MerchantInterface::getMerchantID() const {
	return m_merchantID;
}

int MerchantInterface::getReputation(const Item* item) const {
	if (contains(m_data.reputation, item->getID())) {
		return m_data.reputation.at(item->getID());
	}
	return 0;
}

bool MerchantInterface::isReputationReached(const Item* item) const {
	return m_core->getReputation(m_data.fraction) >= getReputation(item);
}

