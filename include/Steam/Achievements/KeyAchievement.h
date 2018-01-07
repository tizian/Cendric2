#pragma once

#include "Steam/Achievement.h"

class KeyAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};