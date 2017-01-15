#pragma once

#include "global.h"
#include "TextProvider.h"
#include "Item.h"
#include "GUI/ItemDescriptionWindow.h"
#include "Structs/MerchantData.h"

class MerchantItemDescriptionWindow : public ItemDescriptionWindow {
public:
	MerchantItemDescriptionWindow(const MerchantData& data);
	void setReputation(int reputation, bool isReached);

protected:
	std::string getGoldText(const Item& item) const override;
	std::string getReputationText(const Item& item) const override;
	std::string getInteractionText(const Item& item) const override;

private:
	float m_goldMultiplier;
	FractionID m_fraction;
	int m_reputation;
};