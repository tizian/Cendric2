#pragma once

enum class SpellID {
	VOID,

	// not really a spell, each and every weapon has it and its attributes are dependent on
	// the weapon and they not modifiable otherwise.
	Chop,

	// Elemental (Default spells)
	FireBall,
	IceBall,
	Telekinesis,
	WindGust,
	AntiGravity,

	// Twilight
	Unlock,
	ShadowTrap,
	Invisibility,
	Flash,
	IcyAmbush,

	// Necromancy
	Fear,
	Leech,
	RaiseTheDead,
	GhostForm,
	SummonGargoyle,

	// Divine
	Light,
	HolyFire,
	DivineShield,
	LeapOfFaith,
	Aureola,

	MAX
};