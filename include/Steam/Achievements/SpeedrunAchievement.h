#pragma once

#include "Steam/Achievement.h"

class SpeedrunAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};