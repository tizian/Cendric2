#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an equipment particle bean, directly from the database
struct ItemEquipmentParticleBean final : DatabaseBean {
	std::string item_id;
	int particle_count;
    bool is_additive_blend_mode;
    float emit_rate;
    std::string texture_path;
    float spawner_radius;
    sf::Vector2f spawner_offset;
    float size_start_min;
    float size_start_max;
    float size_end_min;
    float size_end_max;
    sf::Color color_start_min;
    sf::Color color_start_max;
    sf::Color color_end_min;
    sf::Color color_end_max;
	float goal_radius;
    sf::Vector2f goal_offset;
    float speed_min;
    float speed_max;
    float time_min;
    float time_max;
	float attract_fraction;
	bool is_climb_hidden;
};