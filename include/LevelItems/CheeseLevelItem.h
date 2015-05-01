#pragma once

#include "global.h"
#include "LevelItem.h"
#include "TextProvider.h"

class CheeseLevelItem : public LevelItem
{
public:
	void load() override;

	LevelItemID getConfiguredItemID() const override;
	const std::string& getConfiguredTooltipText() const override;

private:
};