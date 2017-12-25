#pragma once

#include "global.h"
#include "Steam/CendricAchievements.h"

class Achievement {
public:
    Achievement();
    virtual ~Achievement();
    
    virtual void notify(const std::string& tag, const std::string& message) = 0;
    
protected:
    AchievementData m_data;
};