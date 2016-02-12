#pragma once

enum class SpellID {
	VOID,

	// not really spells, each and every weapon has one of them and their attributes are dependent on
	// the weapon and they not modifiable otherwise.
	Chop,
	Projectile,

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