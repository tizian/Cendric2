#pragma once

#include "Steam/Achievement.h"

class GemAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};A