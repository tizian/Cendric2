#include "GUI/MerchantItemDescriptionWindow.h"
#include "GUI/GUIConstants.h"
#include "Enums/EnumNames.h"

MerchantItemDescriptionWindow::MerchantItemDescriptionWindow(const MerchantData& data)
: ItemDescriptionWindow(), Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	GUIOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_fraction = data.fraction;
	m_reputation = 0;
}

void MerchantItemDescriptionWindow::setReputation(int reputation, bool isReached) {
	m_reputation = reputation;
	m_isReputationReached = isReached;
}

std::string MerchantItemDescriptionWindow::getGoldText(const Item& item, float goldMultiplier) const {
	std::string text;
	text.append(g_textProvider->getText("Price"));
	text.append(": ");
	text.append(std::to_string(static_cast<int>(std::ceil(item.getValue() * goldMultiplier))));
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

std::string MerchantItemDescriptionWindow::getInteractionText(const Item& item, bool isSelling) const {
	return m_isReputationReached ? g_textProvider->getText("RightClickBuy") : "";
}