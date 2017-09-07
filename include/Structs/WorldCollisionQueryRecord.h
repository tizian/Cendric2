#pragma once

#include <SFML/Graphics.hpp>

class GameObject;
class MovingParent;

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
struct WorldCollisionQueryRecord final {
	// the bounding box to be checked for collision
	sf::FloatRect boundingBox; 
	// if set to true, the collision check will not check for collidable dynamic tiles 
	// (but strictly collidable dynamic tiles). Default is false
	bool ignoreDynamicTiles = false;
	// if set to false, the collision check will not include bounding boxes of mobs in the world.
	// default is true.
	bool ignoreMobs = true;
	// if this is set to true, the collision check will not include oneway tiles
	bool ignoreOnewayTiles = false;
	// this is set to the moving platform parent of a movable game object if it hits one.
	MovingParent* movingParent = nullptr;
	// this game object will not be checked for collision
	// used to avoid self-intersections
	GameObject* excludedGameObject = nullptr;

	// what collision direction shall be checked?
	// Up and Down will yield a value in "safe top"
	// Left and Right will yield a value in "safe left" 
	CollisionDirection collisionDirection = CollisionDirection::VOID;
	float safeTop = -1.f;
	float safeLeft = -1.f;

	bool collides = false;
	bool noSafePos = false;
};