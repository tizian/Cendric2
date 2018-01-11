#pragma once

#include "Steam/Achievement.h"

class GrimoireAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	int m_summonedSkeletons = 0;
	bool m_isEnoughSkeletons = false;
};