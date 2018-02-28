#include "Structs/SpellData.h"
#include "Spells/SpellCreator.h"
#include "Spells/ProjectileSpell.h"
#include "Spells/FireBallSpell.h"
#include "Spells/IceBallSpell.h"
#include "Spells/ChopSpell.h"
#include "Spells/LeechSpell.h"
#include "Spells/TransformBeamSpell.h"
#include "Spells/ReturningProjectileSpell.h"
#include "Spells/RotatingProjectileSpell.h"
#include "Spells/TargetingProjectileSpell.h"
#include "Spells/BoomerangSpell.h"
#include "Spells/ExplosionSpell.h"
#include "Spells/ShackleSpell.h"
#include "SpellCreators/DivineShieldSpellCreator.h"
#include "SpellCreators/AureolaSpellCreator.h"
#include "SpellCreators/FearSpellCreator.h"
#include "SpellCreators/AntiGravitySpellCreator.h"
#include "SpellCreators/WindGustSpellCreator.h"
#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "SpellCreators/UnlockSpellCreator.h"
#include "SpellCreators/LightSpellCreator.h"
#include "SpellCreators/TelekinesisSpellCreator.h"
#include "SpellCreators/InvisibilitySpellCreator.h"
#include "SpellCreators/FlashSpellCreator.h"
#include "SpellCreators/LeapOfFaithSpellCreator.h"
#include "SpellCreators/GhostFormSpellCreator.h"
#include "SpellCreators/ShadowTrapSpellCreator.h"
#include "SpellCreators/RaiseTheDeadSpellCreator.h"
#include "SpellCreators/HolyFireSpellCreator.h"
#include "SpellCreators/SummonGargoyleSpellCreator.h"
#include "SpellCreators/DefaultSpellCreator.h"
#include "SpellCreators/BuffSpellCreator.h"

std::vector<SpellModifierType> SpellData::getAllowedModifiers(SpellID id) {
	std::vector<SpellModifierType> types;
	switch (id) {
	case SpellID::Chop:
		break;
	case SpellID::FireBall:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IceBall:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::Aureola:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Speed);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::DivineShield:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Fear:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::WindGust:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Leech:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IcyAmbush:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Unlock:
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Light:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Telekinesis:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Reflect);
		break;
	case SpellID::Invisibility:
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Flash:
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::LeapOfFaith:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::GhostForm:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::ShadowTrap:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::RaiseTheDead:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::HolyFire:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::SummonGargoyle:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Strength);
		break;
	default:
		break;
	}
	return types;
}

SpellCreator* SpellData::getSpellCreator(const SpellData& data, const std::vector<SpellModifier>& modifiers, LevelMovableGameObject* owner) {
	SpellCreator* creator;
	switch (data.id) {
	case SpellID::Chop:
		creator = new DefaultSpellCreator<ChopSpell>(data, owner);
		break;
	case SpellID::FireBall:
		creator = new DefaultSpellCreator<FireBallSpell>(data, owner);
		break;
	case SpellID::IceBall:
		creator = new DefaultSpellCreator<IceBallSpell>(data, owner);
		break;
	case SpellID::DivineShield:
		creator = new DivineShieldSpellCreator(data, owner);
		break;
	case SpellID::Aureola:
		creator = new AureolaSpellCreator(data, owner);
		break;
	case SpellID::Fear:
		creator = new FearSpellCreator(data, owner);
		break;
	case SpellID::AntiGravity:
		creator = new AntiGravitySpellCreator(data, owner);
		break;
	case SpellID::WindGust:
		creator = new WindGustSpellCreator(data, owner);
		break;
	case SpellID::Leech:
		creator = new DefaultSpellCreator<LeechSpell>(data, owner);
		break;
	case SpellID::IcyAmbush:
		creator = new IcyAmbushSpellCreator(data, owner);
		break;
	case SpellID::Unlock:
		creator = new UnlockSpellCreator(data, owner);
		break;
	case SpellID::Light:
		creator = new LightSpellCreator(data, owner);
		break;
	case SpellID::Telekinesis:
		creator = new TelekinesisSpellCreator(data, owner);
		break;
	case SpellID::Invisibility:
		creator = new InvisibilitySpellCreator(data, owner);
		break;
	case SpellID::Flash:
		creator = new FlashSpellCreator(data, owner);
		break;
	case SpellID::LeapOfFaith:
		creator = new LeapOfFaithSpellCreator(data, owner);
		break;
	case SpellID::GhostForm:
		creator = new GhostFormSpellCreator(data, owner);
		break;
	case SpellID::ShadowTrap:
		creator = new ShadowTrapSpellCreator(data, owner);
		break;
	case SpellID::RaiseTheDead:
		creator = new RaiseTheDeadSpellCreator(data, owner);
		break;
	case SpellID::HolyFire:
		creator = new HolyFireSpellCreator(data, owner);
		break;
	case SpellID::SummonGargoyle:
		creator = new SummonGargoyleSpellCreator(data, owner);
		break;
	case SpellID::Projectile:
		creator = new DefaultSpellCreator<ProjectileSpell>(data, owner);
		break;
	case SpellID::ReturningProjectile:
		creator = new DefaultSpellCreator<ReturningProjectileSpell>(data, owner);
		break;
	case SpellID::Boomerang:
		creator = new DefaultSpellCreator<BoomerangSpell>(data, owner);
		break;
	case SpellID::Buff:
		creator = new BuffSpellCreator(data, owner);
		break;
	case SpellID::TransformBeam:
		creator = new DefaultSpellCreator<TransformBeamSpell>(data, owner);
		break;
	case SpellID::RotatingProjectile:
		creator = new DefaultSpellCreator<RotatingProjectileSpell>(data, owner);
		break;
	case SpellID::TargetingProjectile:
		creator = new DefaultSpellCreator<TargetingProjectileSpell>(data, owner);
		break;
	case SpellID::Explosion:
		creator = new DefaultSpellCreator<ExplosionSpell>(data, owner);
		break;
	case SpellID::Shackle:
		creator = new DefaultSpellCreator<ShackleSpell>(data, owner);
		break;
	default:
		return nullptr;
	}

	creator->addModifiers(modifiers);

	return creator;
}

SpellData SpellData::getSpellData(SpellID id) {
	switch (id) {
	case SpellID::Chop:
		return getChopSpellData();
	case SpellID::FireBall:
		return getFireBallSpellData();
	case SpellID::IceBall:
		return getIceBallSpellData();
	case SpellID::Aureola:
		return getAureolaSpellData();
	case SpellID::DivineShield:
		return getDivineShieldSpellData();
	case SpellID::Fear:
		return getFearSpellData();
	case SpellID::AntiGravity:
		return getAntiGravitySpellData();
	case SpellID::WindGust:
		return getWindGustSpellData();
	case SpellID::Leech:
		return getLeechSpellData();
	case SpellID::IcyAmbush:
		return getIcyAmbushSpellData();
	case SpellID::Unlock:
		return getUnlockSpellData();
	case SpellID::Light:
		return getLightSpellData();
	case SpellID::Telekinesis:
		return getTelekinesisSpellData();
	case SpellID::Invisibility:
		return getInvisibilitySpellData();
	case SpellID::Flash:
		return getFlashSpellData();
	case SpellID::LeapOfFaith:
		return getLeapOfFaithSpellData();
	case SpellID::GhostForm:
		return getGhostFormSpellData();
	case SpellID::ShadowTrap:
		return getShadowTrapSpellData();
	case SpellID::RaiseTheDead:
		return getRaiseTheDeadSpellData();
	case SpellID::HolyFire:
		return getHolyFireSpellData();
	case SpellID::SummonGargoyle:
		return getSummonGargoyleSpellData();
	case SpellID::Projectile:
		return getProjectileSpellData();
	case SpellID::ReturningProjectile:
		return getReturningProjectileSpellData();
	case SpellID::Boomerang:
		return getBoomerangSpellData();
	case SpellID::RotatingProjectile:
		return getRotatingProjectileData();
	case SpellID::Buff:
		return getBuffSpellData();
	case SpellID::TargetingProjectile:
		return getTargetingProjectileData();
	default:
		return SpellData();
	}
}

SpellData SpellData::getChopSpellData() {
	SpellData chop;
	chop.id = SpellID::Chop;

	chop.cooldown = sf::milliseconds(400);
	chop.boundingBox = sf::FloatRect(0, 0, 40, 80);
	chop.damageType = DamageType::Physical;
	chop.damage = 1;
	chop.activeDuration = sf::milliseconds(320);
	chop.attachedToMob = true;
	chop.isDynamicTileEffect = true;

	chop.inputKey = Key::Chop;

	return chop;
}

SpellData SpellData::getProjectileSpellData() {
	SpellData projectile;
	projectile.id = SpellID::Projectile;
	projectile.spritesheetPath = "res/texture/spells/spritesheet_spell_projectile.png";
	projectile.creatorSoundPaths = { "res/sound/spell/arrow.ogg" };

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 10, 10);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(5);
	projectile.damagePerSecond = 2;
	projectile.duration = sf::seconds(3);
	projectile.needsTarget = true;
	projectile.isDynamicTileEffect = true;
	projectile.damage = 10;
	projectile.speed = 600.f;

	projectile.inputKey = Key::Chop;

	return projectile;
}

SpellData SpellData::getReturningProjectileSpellData() {
	SpellData projectile;
	projectile.id = SpellID::ReturningProjectile;
	projectile.spritesheetPath = "res/texture/spells/spritesheet_spell_returningprojectile.png";

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 30, 20);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(5);
	projectile.damagePerSecond = 2;
	projectile.duration = sf::seconds(3);
	projectile.needsTarget = true;
	projectile.range = 300;
	projectile.damage = 10;
	projectile.speed = 600.f;

	return projectile;
}

SpellData SpellData::getRotatingProjectileData() {
	SpellData projectile;
	projectile.id = SpellID::RotatingProjectile;
	projectile.spritesheetPath = "res/texture/spells/spritesheet_spell_rotating_projectile.png";

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 35, 35);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(6);
	projectile.needsTarget = true;
	projectile.isDynamicTileEffect = true;
	projectile.damage = 3;
	projectile.speed = 600.f;

	return projectile;
}

SpellData SpellData::getTargetingProjectileData() {
	SpellData projectile;
	projectile.id = SpellID::TargetingProjectile;
	projectile.spritesheetPath = "res/texture/spells/spritesheet_spell_targetingprojectile.png";

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 19, 19);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(6);
	projectile.needsTarget = true;
	projectile.damage = 3;
	projectile.speed = 600.f;
	projectile.strength = 1; // how fast it follows its target

	return projectile;
}

SpellData SpellData::getBoomerangSpellData() {
	SpellData projectile;
	projectile.id = SpellID::Boomerang;
	projectile.spritesheetPath = "res/texture/spells/spritesheet_spell_boomerang.png";

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 10, 10);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(5);
	projectile.damagePerSecond = 2;
	projectile.duration = sf::seconds(3);
	projectile.needsTarget = true;
	projectile.range = 100;
	projectile.damage = 10;
	projectile.speed = 600.f;

	return projectile;
}

SpellData SpellData::getFireBallSpellData() {
	SpellData fireBall;
	fireBall.id = SpellID::FireBall;
	fireBall.spellType = SpellType::Elemental;
	fireBall.spritesheetPath = "res/texture/spells/spritesheet_spell_fireball.png";
	fireBall.creatorSoundPaths = { "res/sound/spell/fireball.ogg" };
	fireBall.iconTextureRect = sf::IntRect(0, 0, 50, 50);

	fireBall.cooldown = sf::seconds(1);
	fireBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fireBall.divergenceAngle = 0.2f;
	fireBall.damageType = DamageType::Fire;
	fireBall.activeDuration = sf::seconds(5);
	fireBall.damagePerSecond = 1;
	fireBall.duration = sf::seconds(3);
	fireBall.needsTarget = true;
	fireBall.isDynamicTileEffect = true;
	fireBall.damage = 10;
	fireBall.speed = 300.f;

	fireBall.countModifierAddition = 1;
	fireBall.reflectModifierAddition = 1;
	fireBall.speedModifierAddition = 100.f;

	return fireBall;
}

SpellData SpellData::getIceBallSpellData() {
	SpellData iceBall;
	iceBall.id = SpellID::IceBall;
	iceBall.spellType = SpellType::Elemental;
	iceBall.spritesheetPath = "res/texture/spells/spritesheet_spell_iceball.png";
	iceBall.iconTextureRect = sf::IntRect(50, 0, 50, 50);

	iceBall.cooldown = sf::seconds(1);
	iceBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	iceBall.divergenceAngle = 0.2f;
	iceBall.damageType = DamageType::Ice;
	iceBall.activeDuration = sf::seconds(5);
	iceBall.isDynamicTileEffect = true;
	iceBall.needsTarget = true;
	iceBall.damage = 10;
	iceBall.speed = 250.f;

	iceBall.countModifierAddition = 1;
	iceBall.reflectModifierAddition = 1;
	iceBall.speedModifierAddition = 100.f;

	return iceBall;
}

SpellData SpellData::getDivineShieldSpellData() {
	SpellData divineShield;
	divineShield.id = SpellID::DivineShield;
	divineShield.spellType = SpellType::Divine;
	divineShield.spritesheetPath = "res/texture/spells/spritesheet_spell_divineshield.png";
	divineShield.iconTextureRect = sf::IntRect(100, 150, 50, 50);

	divineShield.cooldown = sf::seconds(20);
	divineShield.boundingBox = sf::FloatRect(0, 0, 98, 98);
	divineShield.spellOffset = sf::Vector2f(-49.f, 0.f);
	divineShield.duration = sf::seconds(3);
	divineShield.activeDuration = divineShield.duration;
	divineShield.heal = 10;
	divineShield.attachedToMob = true;
	divineShield.fightAnimation = GameObjectState::VOID;

	divineShield.durationModifierAddition = sf::seconds(2);

	return divineShield;
}

SpellData SpellData::getBuffSpellData() {
	SpellData buffSpell;
	buffSpell.id = SpellID::Buff;
	buffSpell.spellType = SpellType::VOID;

	buffSpell.cooldown = sf::milliseconds(10000);
	buffSpell.boundingBox = sf::FloatRect(0, 0, 200, 200);
	buffSpell.duration = sf::seconds(5.f);
	buffSpell.activeDuration = sf::seconds(1.f);
	buffSpell.damage = 20;
	buffSpell.attachedToMob = true;
	buffSpell.fightAnimation = GameObjectState::VOID;

	return buffSpell;
}

SpellData SpellData::getAureolaSpellData() {
	SpellData aureola;
	aureola.id = SpellID::Aureola;
	aureola.spellType = SpellType::Divine;
	aureola.spritesheetPath = "res/texture/spells/spritesheet_spell_aureola.png";
	aureola.iconTextureRect = sf::IntRect(200, 150, 50, 50);

	aureola.cooldown = sf::milliseconds(10000);
	aureola.boundingBox = sf::FloatRect(0, 0, 20, 20);
	aureola.spellOffset = sf::Vector2f(0, 0);
	aureola.count = 4;
	aureola.divergenceAngle = 2 * M_PI / aureola.count;
	aureola.damageType = DamageType::Light;
	aureola.activeDuration = sf::seconds(6);
	aureola.needsTarget = true;
	aureola.duration = sf::seconds(1.f);
	aureola.isStunning = true;
	aureola.ccStrength = 1;
	aureola.damage = 20;
	aureola.heal = 20;
	aureola.speed = 300.f;
	aureola.range = 150.f;

	aureola.countModifierAddition = 2;
	aureola.speedModifierAddition = 100.f;
	aureola.rangeModifierAddition = 100.f;
	aureola.durationModifierAddition = sf::seconds(0.5f);

	return aureola;
}

SpellData SpellData::getFearSpellData() {
	SpellData fear;
	fear.id = SpellID::Fear;
	fear.spellType = SpellType::Necromancy;
	fear.spritesheetPath = "res/texture/spells/spritesheet_spell_fear.png";
	fear.iconTextureRect = sf::IntRect(0, 100, 50, 50);

	fear.cooldown = sf::seconds(3);
	fear.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fear.divergenceAngle = 0.2f;
	fear.activeDuration = sf::seconds(5);
	fear.duration = sf::seconds(2);
	fear.needsTarget = true;
	fear.speed = 300.f;
	fear.isFearing = true;
	fear.ccStrength = 1;

	fear.countModifierAddition = 1;
	fear.reflectModifierAddition = 1;
	fear.speedModifierAddition = 100.f;
	fear.durationModifierAddition = sf::seconds(2);

	return fear;
}

SpellData SpellData::getAntiGravitySpellData() {
	SpellData antiGravity;
	antiGravity.id = SpellID::AntiGravity;
	antiGravity.spellType = SpellType::Elemental;
	antiGravity.iconTextureRect = sf::IntRect(200, 0, 50, 50);

	antiGravity.boundingBox = sf::FloatRect(0, 0, 98, 98);
	antiGravity.spellOffset = sf::Vector2f(-49.f, 0.f);
	antiGravity.cooldown = sf::seconds(2.f);
	antiGravity.duration = sf::seconds(-1);
	antiGravity.activeDuration = antiGravity.duration;
	antiGravity.attachedToMob = true;
	antiGravity.fightAnimation = GameObjectState::VOID;

	return antiGravity;
}

SpellData SpellData::getTelekinesisSpellData() {
	SpellData telekinesis;
	telekinesis.id = SpellID::Telekinesis;
	telekinesis.spellType = SpellType::Elemental;

	telekinesis.spritesheetPath = "res/texture/spells/spritesheet_spell_telekinesis.png";
	telekinesis.iconTextureRect = sf::IntRect(100, 0, 50, 50);
	telekinesis.cooldown = sf::seconds(3);
	telekinesis.boundingBox = sf::FloatRect(0, 0, 20, 20);
	telekinesis.speed = 200.f;
	telekinesis.isDynamicTileEffect = true;
	telekinesis.needsTarget = true;
	telekinesis.range = 150;
	telekinesis.activeDuration = sf::seconds(telekinesis.range / telekinesis.speed);

	telekinesis.rangeModifierAddition = 100.f;
	telekinesis.reflectModifierAddition = 1;

	return telekinesis;
}

SpellData SpellData::getWindGustSpellData() {
	SpellData windGust;
	windGust.id = SpellID::WindGust;
	windGust.spellType = SpellType::Elemental;

	windGust.iconTextureRect = sf::IntRect(150, 0, 50, 50);
	windGust.cooldown = sf::seconds(3);
	windGust.range = 105.f;
	windGust.boundingBox = sf::FloatRect(0.f, 0.f, windGust.range, 60.f);
	windGust.spellOffset = sf::Vector2f(10.f, 10.f);
	windGust.duration = sf::seconds(1.f);
	windGust.activeDuration = windGust.duration;
	windGust.attachedToMob = true;
	windGust.isDynamicTileEffect = true;
	windGust.damageType = DamageType::Ice;
	windGust.damagePerSecond = 3;
	windGust.ccStrength = 1;
	windGust.strength = 1;
	windGust.spellSoundPaths = { "res/sound/spell/windgust.ogg" };

	windGust.rangeModifierAddition = 40.f;
	windGust.durationModifierAddition = sf::seconds(0.5f);

	return windGust;
}

SpellData SpellData::getLeechSpellData() {
	SpellData leech;
	leech.id = SpellID::Leech;
	leech.spellType = SpellType::Necromancy;
	leech.iconTextureRect = sf::IntRect(50, 100, 50, 50);

	leech.cooldown = sf::seconds(3);
	leech.boundingBox = sf::FloatRect(0, 0, 10, 10);
	leech.divergenceAngle = 0.2f;
	leech.damageType = DamageType::Shadow;
	leech.activeDuration = sf::seconds(5);
	leech.needsTarget = true;
	leech.damage = 10;
	leech.heal = 5;
	leech.speed = 150.f;

	leech.countModifierAddition = 1;
	leech.reflectModifierAddition = 1;
	leech.speedModifierAddition = 50.f;

	return leech;
}

SpellData SpellData::getIcyAmbushSpellData() {
	SpellData icyAmbush;
	icyAmbush.id = SpellID::IcyAmbush;
	icyAmbush.spellType = SpellType::Twilight;
	icyAmbush.spritesheetPath = "res/texture/spells/spritesheet_spell_icyambush.png";
	icyAmbush.iconTextureRect = sf::IntRect(200, 50, 50, 50);

	icyAmbush.cooldown = sf::seconds(5);
	icyAmbush.boundingBox = sf::FloatRect(0, 0, 20, 20);
	icyAmbush.damageType = DamageType::Ice;
	icyAmbush.range = 300.f;
	icyAmbush.speed = 200.f;
	icyAmbush.activeDuration = sf::seconds(icyAmbush.range / icyAmbush.speed);
	icyAmbush.duration = sf::seconds(1.5f); // stun duration;
	icyAmbush.needsTarget = true;
	icyAmbush.damage = 20;
	icyAmbush.isStunning = true;
	icyAmbush.ccStrength = 1;
	
	icyAmbush.reflectModifierAddition = 1;
	icyAmbush.rangeModifierAddition = 200.f;
	icyAmbush.durationModifierAddition = sf::seconds(0.5f);

	return icyAmbush;
}

SpellData SpellData::getFlashSpellData() {
	SpellData flash;
	flash.id = SpellID::Flash;
	flash.spellType = SpellType::Twilight;
	flash.spritesheetPath = "res/texture/spells/spritesheet_spell_flash.png";
	flash.iconTextureRect = sf::IntRect(150, 50, 50, 50);

	flash.cooldown = sf::seconds(7);
	flash.damageType = DamageType::Light;
	flash.activeDuration = sf::seconds(2.0);
	flash.attachedToMob = true;
	flash.damage = 10;
	flash.range = 200.f;
	flash.boundingBox = sf::FloatRect(0, 0, 100, 120);
	flash.spellOffset = sf::Vector2f(-50.f, -20.f);
	flash.fightAnimation = GameObjectState::VOID;

	flash.rangeModifierAddition = 80.f;

	return flash;
}

SpellData SpellData::getLightSpellData() {
	SpellData light;
	light.id = SpellID::Light;
	light.spellType = SpellType::Divine;
	light.iconTextureRect = sf::IntRect(0, 150, 50, 50);

	light.cooldown = sf::seconds(60);
	light.boundingBox = sf::FloatRect(0, 0, 1, 1);
	light.spellOffset = sf::Vector2f(0.f, -60.f);
	light.duration = sf::seconds(60);
	light.activeDuration = light.duration;
	light.range = 400.f;
	light.attachedToMob = true;
	light.fightAnimation = GameObjectState::VOID;
	light.isDynamicTileEffect = true;

	light.durationModifierAddition = sf::seconds(60);
	light.rangeModifierAddition = 200.f;

	return light;
}

SpellData SpellData::getHolyFireSpellData() {
	SpellData holyFire;
	holyFire.id = SpellID::HolyFire;
	holyFire.spellType = SpellType::Divine;
	holyFire.iconTextureRect = sf::IntRect(50, 150, 50, 50);

	holyFire.cooldown = sf::seconds(10);
	holyFire.range = 100.f;
	holyFire.boundingBox = sf::FloatRect(0, 0, 2 * holyFire.range, 2 * holyFire.range);
	holyFire.damagePerSecond = 5;
	holyFire.damageType = DamageType::Light;
	holyFire.duration = sf::seconds(2);
	holyFire.activeDuration = sf::seconds(3);
	holyFire.attachedToMob = true;
	holyFire.fightAnimation = GameObjectState::VOID;
	holyFire.spellOffset = sf::Vector2f(-1 * holyFire.range, -1 * holyFire.range);

	holyFire.durationModifierAddition = sf::seconds(1);
	holyFire.rangeModifierAddition = 50.f;

	return holyFire;
}

SpellData SpellData::getLeapOfFaithSpellData() {
	SpellData leapOfFaith;
	leapOfFaith.id = SpellID::LeapOfFaith;
	leapOfFaith.spellType = SpellType::Divine;
	leapOfFaith.spritesheetPath = "res/texture/spells/spritesheet_spell_leapoffaith.png";
	leapOfFaith.iconTextureRect = sf::IntRect(150, 150, 50, 50);

	leapOfFaith.cooldown = sf::seconds(30);
	leapOfFaith.boundingBox = sf::FloatRect(0, 0, 80, 120);
	leapOfFaith.spellOffset = sf::Vector2f(-40.f, -26.f);
	leapOfFaith.duration = sf::seconds(10);
	leapOfFaith.activeDuration = leapOfFaith.duration;
	leapOfFaith.attachedToMob = true;
	leapOfFaith.fightAnimation = GameObjectState::VOID;
	leapOfFaith.strength = 1;

	leapOfFaith.durationModifierAddition = sf::seconds(10);

	return leapOfFaith;
}

SpellData SpellData::getUnlockSpellData() {
	SpellData unlock;
	unlock.id = SpellID::Unlock;
	unlock.spellType = SpellType::Twilight;
	unlock.spritesheetPath = "res/texture/spells/spritesheet_spell_unlock.png";
	unlock.iconTextureRect = sf::IntRect(0, 50, 50, 50);

	unlock.cooldown = sf::seconds(2);
	unlock.boundingBox = sf::FloatRect(0, 0, 10, 10);
	unlock.activeDuration = sf::seconds(1);
	unlock.needsTarget = true;
	unlock.isDynamicTileEffect = true;
	unlock.speed = 200.f;
	unlock.strength = 1;

	return unlock;
}

SpellData SpellData::getInvisibilitySpellData() {
	SpellData invisibility;
	invisibility.id = SpellID::Invisibility;
	invisibility.spellType = SpellType::Twilight;
	invisibility.iconTextureRect = sf::IntRect(100, 50, 50, 50);

	invisibility.cooldown = sf::seconds(30);
	invisibility.boundingBox = sf::FloatRect(0, 0, 1, 1);
	invisibility.spellOffset = sf::Vector2f(0.f, 80.f);
	invisibility.activeDuration = sf::seconds(5);
	invisibility.duration = invisibility.activeDuration;
	invisibility.attachedToMob = true;
	invisibility.fightAnimation = GameObjectState::VOID;
	invisibility.strength = 1;

	invisibility.durationModifierAddition = sf::seconds(5);

	return invisibility;
}

SpellData SpellData::getGhostFormSpellData() {
	SpellData ghostForm;
	ghostForm.id = SpellID::GhostForm;
	ghostForm.spellType = SpellType::Necromancy;

	ghostForm.spritesheetPath = "res/texture/spells/spritesheet_spell_ghost.png";
	ghostForm.iconTextureRect = sf::IntRect(150, 100, 50, 50);
	ghostForm.cooldown = sf::seconds(20);
	ghostForm.boundingBox = sf::FloatRect(0, 0, 30, 80);
	ghostForm.spellOffset = sf::Vector2f(-20.f, 0.f);
	ghostForm.activeDuration = sf::seconds(4);
	ghostForm.duration = ghostForm.activeDuration;
	ghostForm.speed = 50.f;
	ghostForm.attachedToMob = true;
	ghostForm.fightAnimation = GameObjectState::VOID;
	ghostForm.strength = 40; // additional physical resistance

	ghostForm.durationModifierAddition = sf::seconds(2);
	ghostForm.speedModifierAddition = 25.f;

	return ghostForm;
}

SpellData SpellData::getShadowTrapSpellData() {
	SpellData shadowTrap;
	shadowTrap.id = SpellID::ShadowTrap;
	shadowTrap.spellType = SpellType::Twilight;
	shadowTrap.spritesheetPath = "res/texture/spells/spritesheet_spell_shadowtrap.png";
	shadowTrap.creatorSoundPaths = { "res/sound/spell/trap.ogg" };
	shadowTrap.iconTextureRect = sf::IntRect(50, 50, 50, 50);

	shadowTrap.cooldown = sf::seconds(10);
	shadowTrap.boundingBox = sf::FloatRect(0, 0, 25, 10);
	shadowTrap.spellOffset = sf::Vector2f(-shadowTrap.boundingBox.width * 0.5f, 10.f);
	shadowTrap.divergenceAngle = 0.2f;
	shadowTrap.damageType = DamageType::Shadow;
	shadowTrap.activeDuration = sf::seconds(15);
	shadowTrap.damagePerSecond = 10;
	shadowTrap.duration = sf::seconds(3);
	shadowTrap.isStunning = true;
	shadowTrap.ccStrength = 1;
	shadowTrap.fightAnimation = GameObjectState::VOID;

	shadowTrap.durationModifierAddition = sf::seconds(1);

	return shadowTrap;
}

SpellData SpellData::getSummonGargoyleSpellData() {
	SpellData summonGargoyle;
	summonGargoyle.id = SpellID::SummonGargoyle;
	summonGargoyle.spellType = SpellType::Necromancy;
	summonGargoyle.creatorSoundPaths = { "res/sound/tile/gargoyle.ogg" };
	summonGargoyle.spritesheetPath = "res/texture/enemies/spritesheet_enemy_gargoyle.png";
	summonGargoyle.iconTextureRect = sf::IntRect(200, 100, 50, 50);

	summonGargoyle.cooldown = sf::seconds(30);
	summonGargoyle.damageType = DamageType::Physical;
	summonGargoyle.damage = 0;
	summonGargoyle.duration = sf::seconds(20);
	summonGargoyle.ccStrength = 1;
	summonGargoyle.fightAnimation = GameObjectState::VOID;

	summonGargoyle.durationModifierAddition = sf::seconds(20);
	summonGargoyle.countModifierAddition = 1;

	return summonGargoyle;
}

SpellData SpellData::getRaiseTheDeadSpellData() {
	SpellData raiseTheDead;
	raiseTheDead.id = SpellID::RaiseTheDead;
	raiseTheDead.spellType = SpellType::Necromancy;

	raiseTheDead.spritesheetPath = "res/texture/spells/spritesheet_spell_raisethedead.png";
	raiseTheDead.iconTextureRect = sf::IntRect(100, 100, 50, 50);
	raiseTheDead.cooldown = sf::seconds(10);
	raiseTheDead.boundingBox = sf::FloatRect(0, 0, 10, 10);
	raiseTheDead.speed = 200.f;
	raiseTheDead.damage = 5;
	raiseTheDead.duration = sf::seconds(30.f);
	raiseTheDead.needsTarget = true;
	raiseTheDead.range = 100;
	raiseTheDead.activeDuration = sf::seconds(raiseTheDead.range / raiseTheDead.speed);
	raiseTheDead.strength = 1;

	raiseTheDead.rangeModifierAddition = 150.f;
	raiseTheDead.durationModifierAddition = sf::seconds(20.f);

	return raiseTheDead;
}
