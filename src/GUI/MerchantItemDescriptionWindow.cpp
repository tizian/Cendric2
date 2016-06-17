#include "GUI/MerchantItemDescriptionWindow.h"

using namespace std;

MerchantItemDescriptionWindow::MerchantItemDescriptionWindow(float goldMultiplier) {
	m_goldMultiplier = goldMultiplier;
}

std::string MerchantItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string text;
	text.append(g_textProvider->getText("Price"));
	text.append(": ");
	text.append(to_string((int)std::ceil(item.getValue() * m_goldMultiplier)));
	return text;
}