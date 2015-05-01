#pragma once

#include "global.h"
#include "LevelItem.h"
#include "TextProvider.h"

class CheeseLevelItem : public LevelItem
{
public:
	void load() override;
	const LevelItemID getConfiguredItemID() override;
	const std::string getConfiguredTooltipText() override;
private:
};