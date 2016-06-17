#pragma once

#include "global.h"
#include "TextProvider.h"
#include "Item.h"
#include "GUI/ItemDescriptionWindow.h"

class MerchantItemDescriptionWindow : public ItemDescriptionWindow {
public:
	MerchantItemDescriptionWindow(float goldMultiplier);

protected:
	std::string getGoldLabelText() const override;
	std::string getGoldValueText(const Item& item) const override;

private:
	float m_goldMultiplier;
};