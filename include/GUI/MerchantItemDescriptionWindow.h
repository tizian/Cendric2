#pragma once

#include "global.h"
#include "TextProvider.h"
#include "World/Item.h"
#include "GUI/ItemDescriptionWindow.h"
#include "Structs/MerchantData.h"

class MerchantItemDescriptionWindow final : public ItemDescriptionWindow {
public:
	MerchantItemDescriptionWindow(const MerchantData& data);
	void setReputation(int reputation, bool isReached);

protected:
	std::string getGoldText(const Item& item, float goldMultiplier) const override;
	std::string getReputationText(const Item& item) const override;
	std::string getInteractionText(const Item& item) const override;

private:
	FractionID m_fraction;
	int m_reputation;
};