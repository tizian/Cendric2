#include "MerchantInterface.h"

MerchantInterface::MerchantInterface(GameScreen* screen, std::string merchantID) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
	m_merchantID = merchantID;
	m_data = m_core->getMerchantData(merchantID);
	m_merchantWindow = new MerchantWindow(this);
	m_inventory = m_screen->getInventory();
	m_inventory->startTrading(this);
}

MerchantInterface::~MerchantInterface() {
	delete m_merchantWindow;
}

void MerchantInterface::completeTrade() {
	if (m_screen->getInventory() != nullptr) m_screen->getInventory()->stopTrading();
	m_core->setMerchantData(m_merchantID, m_data);
	m_isCancelled = true;
}

void MerchantInterface::sellItem(const Item& item) {
	if (item.getBean().goldValue < 0) {
		m_screen->setTooltipText(g_textProvider->getText("Unsalable"), sf::Color::Red, true);
		return;
	}
	m_screen->notifyItemChange("gold", item.getBean().goldValue);
	if (m_data.wares.find(item.getID()) == m_data.wares.end()) {
		m_data.wares.insert({ item.getID(), 1 });
	}
	else {
		int amount = m_data.wares.at(item.getID());
		amount++;
		m_data.wares[item.getID()] = amount;
	}
	m_merchantWindow->notifyChange(item.getID());
	m_screen->notifyItemChange(item.getID(), -1);
}

void MerchantInterface::buyItem(const Item& item) {
	if (m_core->getData().gold < (int)std::ceil(m_data.multiplier * item.getBean().goldValue)) {
		m_screen->setTooltipText(g_textProvider->getText("NotEnoughGold"), sf::Color::Red, true);
		return;
	}
	m_screen->notifyItemChange("gold", -(int)std::ceil(m_data.multiplier * item.getBean().goldValue));
	m_screen->notifyItemChange(item.getID(), 1);
	if (m_data.wares.find(item.getID()) != m_data.wares.end()) {
		int amount = m_data.wares.at(item.getID());
		amount--;
		if (amount <= 0) {
			m_data.wares.erase(m_data.wares.find(item.getID()));
		}
		else {
			m_data.wares[item.getID()] = amount;
		}
	}
	m_merchantWindow->notifyChange(item.getID());
}

bool MerchantInterface::isCancelled() {
	return m_isCancelled;
}

void MerchantInterface::render(sf::RenderTarget& target) {
	m_merchantWindow->render(target);
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

