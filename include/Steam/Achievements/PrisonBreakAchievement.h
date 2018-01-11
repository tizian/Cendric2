#pragma once

#include "Steam/Achievement.h"

class PrisonBreakAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	bool m_isPart1Done = false;
};