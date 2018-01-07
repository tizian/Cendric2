#pragma once

#include "Steam/Achievement.h"

class MasochistAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};