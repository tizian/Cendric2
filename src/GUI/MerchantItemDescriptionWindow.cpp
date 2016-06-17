#include "GUI/MerchantItemDescriptionWindow.h"

using namespace std;

MerchantItemDescriptionWindow::MerchantItemDescriptionWindow(float goldMultiplier) {
	m_goldMultiplier = goldMultiplier;
}

std::string MerchantItemDescriptionWindow::getGoldLabelText() const {
	std::string text;
	text.append(g_textProvider->getText("Price"));
	text.append(":\n");
	return text;
}

std::string MerchantItemDescriptionWindow::getGoldValueText(const Item& item) const {
	std::string text;
	text.append(to_string((int)std::ceil(item.getValue() * m_goldMultiplier)));
	text.append("\n");
	return text;
}