#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an equipment particle bean, directly from the database
struct EquipmentParticleBean final : DatabaseBean {
	std::string item_id;
	int particle_count;
    bool is_additive_blend_mode;
    int emit_rate;
    std::string texture_path;
    float spawner_radius;
    sf::Vector2f spawner_offset;
    int size_start_min;
    int size_start_max;
    int size_end_min;
    int size_end_max;
    sf::Color color_start_min;
    sf::Color color_start_max;
    sf::Color color_end_min;
    sf::Color color_end_max;
    sf::Vector2f goal_offset;
    int speed_min;
    int speed_max;
    float time_min;
    float time_max;
};