#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

// super struct for all database beans
struct DatabaseBean {
	virtual ~DatabaseBean() {}
};