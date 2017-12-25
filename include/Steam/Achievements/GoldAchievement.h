#pragma once

#include "Steam/Achievement.h"

class GoldAchievement final : public Achievement {
public:
    void notify(const std::string& tag, const std::string& message) override;
};