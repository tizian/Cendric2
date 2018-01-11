#pragma once

#include "Steam/Achievement.h"

class LeverAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	bool m_hasStarted = false;
	bool m_currentTurns = 0;
};