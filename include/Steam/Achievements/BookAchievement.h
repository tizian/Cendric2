#pragma once

#include "Steam/Achievement.h"

class BookAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};