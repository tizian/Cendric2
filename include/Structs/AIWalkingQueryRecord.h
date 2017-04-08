#pragma once

#include <SFML/Graphics.hpp>

/*
used by walking enemies to check whether they should jump and walk or only walk or stop
*/
struct AIWalkingQueryRecord {
	// the bounding box to be checked for collision
	sf::FloatRect boundingBox;
	// if set to true, the collision check will not check for collidable dynamic tiles 
	// (but strictly collidable dynamic tiles). Default is false
	bool ignoreDynamicTiles = false;
	bool isFlippedGravity = false;
	bool isDropAlways = false;
	// the acceleration in x direction (walking)
	float accelerationX = 0.f;
	// the gravity acceleration (must  be mirrored if the mob is upside down)
	float accelerationGravity = 0.f;
	float dampingGroundPerS = 1.f;
	float dampingAirPerS = 0.f;

	float maxVelX = 0.f;
	float maxVelYDown = 0.f;
	float maxVelYUp = 0.f;

	// the calculated jump height for a mob
	float jumpHeight;

	// how close does a mob go to the abyss before it stops?
	float distanceToAbyss = 0.f;

	// should the enemy jump (and walk) to reach its goal?
	bool shouldJump = false;
	// should the enemy walk on?
	bool shouldWalk = false;
};