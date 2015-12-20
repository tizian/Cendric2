#pragma once

// the type of a game object.
// used by the screen for rendering and update order.
// MAX should not be used as enum types as it is only used for the enum iterator
// undefined should always be the first (=0). It can be used though.
enum GameObjectType {
	_Undefined,
	_MainCharacter,
	_LevelEquipment, // includes everything cendric can wear
	_Button,
	_Window,
	_Form, // form buttons will be rendered / updated with their form and not in the "button" context
	_Spell,
	_DynamicTile, // dynamic tiles on map and levels
	_Enemy,
	_LevelItem,
	_NPC,
	_Interface,
	_Light,
	_AnimatedTile,
	_MAX
};