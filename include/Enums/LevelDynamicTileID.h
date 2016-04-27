#pragma once

// NOTE: don't change this enum order. They are aligned with the level dynamic tile tileset.
enum class LevelDynamicTileID {
	VOID,
	Destructible,
	Ice,
	Fluid,
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
	Unstable,
	Falling,
	Jumping,
	Shooting,
	Sign,
	MAX
};