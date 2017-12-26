#pragma once

#include "Steam/Achievement.h"

class GoldAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};