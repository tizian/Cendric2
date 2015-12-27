#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum class BeanStatus {
	NotSet,
	Filled
};

// super struct for all database beans, initializes with not set.
struct DatabaseBean {
	BeanStatus status = BeanStatus::NotSet;
};