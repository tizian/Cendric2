#pragma once

#include "Steam/Achievement.h"

class CanadianLoopAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	int m_loopCount = 0;
};