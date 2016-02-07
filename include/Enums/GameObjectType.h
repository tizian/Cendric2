#pragma once

// the type of a game object.
// used by the screen for rendering and update order.
// MAX should not be used as enum types as it is only used for the enum iterator
// undefined should always be the first (=0). It can be used though.
enum GameObjectType {
	_Undefined,
	_LevelMainCharacter,
	_LevelEquipment, // includes everything cendric can wear
	_Button,
	_Window,
	_Form, // form buttons will be rendered / updated with their form and not in the "button" context
	_Spell,
	_DynamicTile, // dynamic tiles on map and levels, an exception are moving platforms, shiftable tiles and unstable tliles.
	_MapMovableGameObject, // npcs and main char on map
	_Enemy,
	_LevelItem,
	_Interface,
	_Light,
	_AnimatedTile,
	_MovableTile, // moving tiles, shiftable tiles and unstable tiles.
	_MAX
};