#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum class BeanStatus {
	Error,
	Filled
};

// super struct for all database beans, initializes with error
struct DatabaseBean {
	BeanStatus status = BeanStatus::Error;
};