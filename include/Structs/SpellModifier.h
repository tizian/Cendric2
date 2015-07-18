#pragma once

enum class SpellModifierType
{
	VOID,
	Damage,
	Range,
	Speed,
	Reflect,
	Count,
	Strength,
	Duration,
	MAX
};

/* a simple spell modifier */
struct SpellModifier
{
	SpellModifierType type;
	// level ranges from 1 to 3
	int level;
};