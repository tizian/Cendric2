#pragma once

// NOTE: don't change this enum order. They are aligned with the level dynamic tile tileset.
enum class LevelDynamicTileID {
	VOID,
	CrumblyBlock,
	Ice,
	Water,
	Torch,
	Chest,
	SpikesBottom,
	SpikesTop,
	ShiftableBlock,
	Checkpoint,
	Lever,
	SwitchableOn,
	SwitchableOff,
	Modifier,
	MAX
};