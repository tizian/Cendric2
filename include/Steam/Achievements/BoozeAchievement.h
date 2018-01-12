#pragma once

#include "Steam/Achievement.h"

class BoozeAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	bool m_isStarted = false;
	int m_boozeAmount = 0;
};