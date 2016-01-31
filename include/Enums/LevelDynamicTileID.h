#pragma once

// NOTE: don't change this enum order. They are aligned with the level dynamic tile tileset.
enum class LevelDynamicTileID {
	VOID,
	Destructible,
	Ice,
	Water,
	Torch,
	Chest,
	SpikesBottom,
	SpikesTop,
	Shiftable,
	Checkpoint,
	Lever,
	SwitchableOn,
	SwitchableOff,
	Modifier,
	Moving,
	MAX
};