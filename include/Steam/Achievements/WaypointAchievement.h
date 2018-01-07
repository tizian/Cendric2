#pragma once

#include "Steam/Achievement.h"

class WaypointAchievement final : public Achievement {
public:
    bool notifyCore(const CharacterCore* core) override;
};