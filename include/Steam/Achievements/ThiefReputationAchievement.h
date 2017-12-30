#pragma once

#include "Steam/Achievement.h"

class ThiefReputationAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};