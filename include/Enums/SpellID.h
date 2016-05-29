#pragma once

enum class SpellID {
	VOID,

	// not really a spell, each and every weapon of cendric has them and it is not modifiable.
	Chop,

	// Elemental (Default spells)
	IceBall,
	FireBall,
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

	// Other, used by enemies
	Buff,
	Projectile,

	MAX
};