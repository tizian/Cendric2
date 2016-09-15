#pragma once

// this enum is used for animations and used by all game objects
enum class GameObjectState {
	VOID,
	Idle, // used by nearly every game object
	Idle_up, // used by map main character
	Idle_down, // used by map main character
	Idle_left, // used by map main character
	Idle_right, // used by map main character
	Walking, // used by mobs and its weapons
	Walking_up, // used by map main character
	Walking_down, // used by map main character
	Walking_right, // used by map main character
	Walking_left, // used by map main character
	Climbing_1, // used by level main character and equipment
	Climbing_2, // used by level main character and equipment
	Flying, // used by flying mobs
	Jumping, // used by mobs and its weapons
	Casting, // used by mobs and its weapons
	Casting2, // used by mobs and its weapons
	Casting3, // used by mobs and its weapons
	Fighting, // used by mobs and its weapons
	Fighting2, // used by mobs and its weapons
	Fighting3, // used by mobs and its weapons
	Dead, // used by mobs and its weapons
	TripOver, // used by mobs and its weapons
	Standup, // used by mobs and its weapons
	Laying, // used by mobs and its weapons
	Frozen, // used by water tile
	Locked, // used by chest tile
	Unlocked, // used by chest tile
	Burning, // used by torch tile
	Crumbling, // used by crumbly block tile
	Broken, // used by shooting tiles and resource tiles
	Active, // used by checkpoint tiles and resource tiles
	On, // used by the lever and the switch tiles
	Off, // used by the lever and the switch tiles
	Trembling, // used by unstable tiles
	Blinking, // used by observer/warden enemies
	Looking, // used by observer/warden enemies
	Open,	// used by door tiles
	Closed	// used by door tiles
};