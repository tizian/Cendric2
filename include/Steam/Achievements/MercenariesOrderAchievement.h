#pragma once

#include "Steam/Achievement.h"

class MercenariesOrderAchievement final : public Achievement {
public:
    bool notify(const std::string& message) override;
private:
	int m_correctOrderState = 0;
};