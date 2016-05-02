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
	Flying, // used by flying mobs
	Jumping, // used by mobs and its weapons
	Fighting, // used by mobs and its weapons
	Fighting2, // used by mobs and its weapons
	Dead, // used by mobs and its weapons
	Frozen, // used by water tile
	Locked, // used by chest tile
	Unlocked, // used by chest tile
	Burning, // used by torch tile
	Crumbling, // used by crumbly block tile
	Broken, // used by shooting tiles
	Active, // used by checkpoint tiles
	On, // used by the lever and the switch tiles
	Off, // used by the lever and the switch tiles
	Trembling // used by unstable tiles
};