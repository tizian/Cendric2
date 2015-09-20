#pragma once

#include "global.h"
#include "TextProvider.h"
#include "Item.h"
#include "GUI/ItemDescriptionWindow.h"

class MerchantItemDescriptionWindow : public ItemDescriptionWindow
{
public:
	MerchantItemDescriptionWindow(float goldMultiplier);

protected:
	std::wstring getGoldText(const Item& item) const override;

private:
	float m_goldMultiplier;
};