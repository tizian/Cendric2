#include "GUI/MerchantItemDescriptionWindow.h"

using namespace std;

MerchantItemDescriptionWindow::MerchantItemDescriptionWindow(float goldMultiplier) {
	m_goldMultiplier = goldMultiplier;
}

std::wstring MerchantItemDescriptionWindow::getGoldText(const Item& item) const {
	std::wstring goldText;
	goldText.append(g_textProvider->getText("Price"));
	goldText.append(L": ");
	goldText.append(to_wstring((int)std::ceil(item.getBean().goldValue * m_goldMultiplier)));
	return goldText;
}
