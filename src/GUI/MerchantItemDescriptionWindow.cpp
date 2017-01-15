#include "GUI/MerchantItemDescriptionWindow.h"

MerchantItemDescriptionWindow::MerchantItemDescriptionWindow(const MerchantData& data) {
	m_goldMultiplier = data.multiplier;
	m_fraction = data.fraction;
	m_reputation = 0;
}

void MerchantItemDescriptionWindow::setReputation(int reputation, bool isReached) {
	m_reputation = reputation;
	m_isReputationReached = isReached;
}

std::string MerchantItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string text;
	text.append(g_textProvider->getText("Price"));
	text.append(": ");
	text.append(std::to_string((int)std::ceil(item.getValue() * m_goldMultiplier)));
	return text;
}

std::string MerchantItemDescriptionWindow::getReputationText(const Item& item) const {
	if (m_fraction == FractionID::VOID || m_reputation == 0) return "";

	std::string text;
	text.append(g_textProvider->getText("Reputation"));
	text.append(" (");
	text.append(g_textProvider->getText(EnumNames::getShortFractionIDName(m_fraction)));
	text.append("): ");
	text.append(std::to_string(m_reputation));

	return text;
}

std::string MerchantItemDescriptionWindow::getInteractionText(const Item& item) const {
	return "";
}