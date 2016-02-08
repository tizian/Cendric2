#pragma once

#include <SFML/Graphics.hpp>

class GameObject;

enum class CollisionDirection {
	VOID,
	Up,
	Left,
	Down,
	Right
};

/*
used by movable game objects to check a collision with the world.
*/
struct WorldCollisionQueryRecord {
	// the bounding box to be checked for collision
	sf::FloatRect boundingBox; 
	// if set to true, the collision check will not check for collidable dynamic tiles 
	// (but strictly collidable dynamic tiles). Default is false
	bool ignoreDynamicTiles = false;
	// if set to false, the collision check will not include bounding boxes of mobs in the world.
	// default is true.
	bool ignoreMobs = true;
	// this vector is set to the relative velocity of a movable game object 
	// if the bounding box collides with it (used for moving platforms)
	sf::Vector2f gainedRelativeVelocity = sf::Vector2f(0.f, 0.f);
	// this game object will not be checked for collision
	// used to avoid self-intersections
	GameObject* excludedGameObject = nullptr;
	// used for the levels moving platforms
	bool checkMovingPlatforms = false;
	// is anti gravity for the object on?
	bool upsideDown = false;

	// what direction are we checking? this direction is set by the world and calculated
	// by the mob velocity and the relative velocity of collisions it encounters.
	// Down and Down will yield a value in "safe top"
	// Left and Right will yield a value in "safe left" 
	CollisionDirection collisionDirection = CollisionDirection::VOID;
	float safeTop = -1.f;
	float safeLeft = -1.f;
};