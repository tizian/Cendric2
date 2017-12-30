#pragma once

#include "Steam/Achievement.h"

class NecroReputationAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};