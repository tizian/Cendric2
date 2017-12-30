#pragma once

#include "Steam/Achievement.h"

class ClericReputationAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};