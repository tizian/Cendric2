#include "Structs/SpellData.h"
#include "SpellCreator.h"
#include "SpellCreators/FireBallSpellCreator.h"
#include "SpellCreators/IceBallSpellCreator.h"
#include "SpellCreators/DivineShieldSpellCreator.h"
#include "SpellCreators/AureolaSpellCreator.h"
#include "SpellCreators/ChopSpellCreator.h"
#include "SpellCreators/FearSpellCreator.h"
#include "SpellCreators/AntiGravitySpellCreator.h"
#include "SpellCreators/WindGustSpellCreator.h"
#include "SpellCreators/LeechSpellCreator.h"
#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "SpellCreators/UnlockSpellCreator.h"
#include "SpellCreators/LightSpellCreator.h"
#include "SpellCreators/TelekinesisSpellCreator.h"
#include "SpellCreators/InvisibilitySpellCreator.h"
#include "SpellCreators/FlashSpellCreator.h"
#include "SpellCreators/LeapOfFaithSpellCreator.h"

std::vector<SpellModifierType> SpellData::getAllowedModifiers(SpellID id) {
	std::vector<SpellModifierType> types;
	switch (id) {
	case SpellID::Chop:
		break;
	case SpellID::FireBall:
		types.push_back(SpellModifierType::Damage);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IceBall:
		types.push_back(SpellModifierType::Damage);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::Aureola:
		types.push_back(SpellModifierType::Damage);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Speed);
		types.push_back(SpellModifierType::Range);
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
	case SpellID::AntiGravity:
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::WindGust:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Leech:
		types.push_back(SpellModifierType::Damage);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IcyAmbush:
		types.push_back(SpellModifierType::Damage);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Reflect);
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
		types.push_back(SpellModifierType::Damage);
		break;
	case SpellID::LeapOfFaith:
		types.push_back(SpellModifierType::Duration);
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
		creator = new ChopSpellCreator(data, owner);
		break;
	case SpellID::FireBall:
		creator = new FireBallSpellCreator(data, owner);
		break;
	case SpellID::IceBall:
		creator = new IceBallSpellCreator(data, owner);
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
		creator = new LeechSpellCreator(data, owner);
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
	default:
		return EMPTY_SPELL;
	}
}

SpellData SpellData::getChopSpellData() {
	SpellData chop = EMPTY_SPELL;
	chop.id = SpellID::Chop;

	chop.cooldown = sf::milliseconds(400);
	chop.boundingBox = sf::FloatRect(0, 0, 40, 80);
	chop.damageType = DamageType::Physical;
	chop.damage = 3;
	chop.activeDuration = sf::milliseconds(320);

	chop.inputKey = Key::Chop;

	return chop;
}

SpellData SpellData::getFireBallSpellData() {
	SpellData fireBall = EMPTY_SPELL;
	fireBall.id = SpellID::FireBall;
	fireBall.spellType = SpellType::Elemental;
	fireBall.iconTextureRect = sf::IntRect(0, 0, 50, 50);

	fireBall.cooldown = sf::seconds(1);
	fireBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fireBall.divergenceAngle = 0.2f;
	fireBall.damageType = DamageType::Fire;
	fireBall.activeDuration = sf::seconds(5);
	fireBall.needsTarget = true;
	fireBall.damage = 10;
	fireBall.startVelocity = 300.f;

	fireBall.countModifierAddition = 1;
	fireBall.damageModifierAddition = 20;
	fireBall.reflectModifierAddition = 1;
	fireBall.speedModifierAddition = 100.f;

	return fireBall;
}

SpellData SpellData::getIceBallSpellData() {
	SpellData iceBall = EMPTY_SPELL;
	iceBall.id = SpellID::IceBall;
	iceBall.spellType = SpellType::Elemental;
	iceBall.iconTextureRect = sf::IntRect(50, 0, 50, 50);

	iceBall.cooldown = sf::seconds(1);
	iceBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	iceBall.divergenceAngle = 0.2f;
	iceBall.damageType = DamageType::Ice;
	iceBall.activeDuration = sf::seconds(5);
	iceBall.needsTarget = true;
	iceBall.damage = 6;
	iceBall.startVelocity = 200.f;

	iceBall.countModifierAddition = 1;
	iceBall.damageModifierAddition = 20;
	iceBall.reflectModifierAddition = 1;
	iceBall.speedModifierAddition = 100.f;

	return iceBall;
}

SpellData SpellData::getDivineShieldSpellData() {
	SpellData divineShield = EMPTY_SPELL;
	divineShield.id = SpellID::DivineShield;
	divineShield.spellType = SpellType::Divine;
	divineShield.iconTextureRect = sf::IntRect(100, 150, 50, 50);

	divineShield.cooldown = sf::milliseconds(10000);
	divineShield.boundingBox = sf::FloatRect(0, 0, 98, 98);
	divineShield.duration = sf::seconds(3);
	divineShield.activeDuration = divineShield.duration;
	divineShield.heal = 20;

	divineShield.durationModifierAddition = sf::seconds(2);

	return divineShield;
}


SpellData SpellData::getAureolaSpellData() {
	SpellData aureola = EMPTY_SPELL;
	aureola.id = SpellID::Aureola;
	aureola.spellType = SpellType::Divine;
	aureola.iconTextureRect = sf::IntRect(200, 150, 50, 50);

	aureola.cooldown = sf::milliseconds(10000);
	aureola.boundingBox = sf::FloatRect(0, 0, 20, 20);
	aureola.count = 4;
	aureola.divergenceAngle = 2 * M_PI / aureola.count;
	aureola.damageType = DamageType::Light;
	aureola.activeDuration = sf::seconds(10);
	aureola.needsTarget = true;
	aureola.damage = 10;
	aureola.heal = 10;
	aureola.startVelocity = 300.f;
	aureola.range = 150.f;

	aureola.countModifierAddition = 2;
	aureola.damageModifierAddition = 10;
	aureola.speedModifierAddition = 100.f;
	aureola.rangeModifierAddition = 100.f;

	return aureola;
}

SpellData SpellData::getFearSpellData() {
	SpellData fear = EMPTY_SPELL;
	fear.id = SpellID::Fear;
	fear.spellType = SpellType::Necromancy;
	fear.iconTextureRect = sf::IntRect(0, 100, 50, 50);

	fear.cooldown = sf::seconds(3);
	fear.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fear.divergenceAngle = 0.2f;
	fear.activeDuration = sf::seconds(5);
	fear.duration = sf::seconds(2);
	fear.needsTarget = true;
	fear.startVelocity = 300.f;

	fear.countModifierAddition = 1;
	fear.reflectModifierAddition = 1;
	fear.speedModifierAddition = 100.f;
	fear.durationModifierAddition = sf::seconds(2);

	return fear;
}

SpellData SpellData::getAntiGravitySpellData() {
	SpellData antiGravity = EMPTY_SPELL;
	antiGravity.id = SpellID::AntiGravity;
	antiGravity.spellType = SpellType::Elemental;
	antiGravity.iconTextureRect = sf::IntRect(200, 0, 50, 50);

	antiGravity.cooldown = sf::seconds(15);
	antiGravity.boundingBox = sf::FloatRect(0, 0, 98, 98);
	antiGravity.duration = sf::seconds(5);
	antiGravity.activeDuration = antiGravity.duration;

	antiGravity.durationModifierAddition = sf::seconds(3);

	return antiGravity;
}

SpellData SpellData::getTelekinesisSpellData() {
	SpellData telekinesis = EMPTY_SPELL;
	telekinesis.id = SpellID::Telekinesis;
	telekinesis.spellType = SpellType::Elemental;

	telekinesis.iconTextureRect = sf::IntRect(100, 0, 50, 50);
	telekinesis.cooldown = sf::seconds(3);
	telekinesis.boundingBox = sf::FloatRect(0, 0, 20, 20);
	telekinesis.startVelocity = 200.f;
	telekinesis.needsTarget = true;
	telekinesis.range = 100;
	telekinesis.activeDuration = sf::seconds(telekinesis.range/telekinesis.startVelocity);

	telekinesis.rangeModifierAddition = 150.f;
	telekinesis.reflectModifierAddition = 1;

	return telekinesis;
}

SpellData SpellData::getWindGustSpellData() {
	SpellData windGust = EMPTY_SPELL;
	windGust.id = SpellID::WindGust;
	windGust.spellType = SpellType::Elemental;

	windGust.iconTextureRect = sf::IntRect(150, 0, 50, 50);
	windGust.cooldown = sf::seconds(3);
	windGust.boundingBox = sf::FloatRect(0, 0, 60, 40);
	windGust.duration = sf::seconds(1);
	windGust.activeDuration = windGust.duration;

	windGust.rangeModifierAddition = 20.f;
	windGust.durationModifierAddition = sf::seconds(1);

	return windGust;
}

SpellData SpellData::getLeechSpellData() {
	SpellData leech = EMPTY_SPELL;
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
	leech.startVelocity = 150.f;

	leech.countModifierAddition = 1;
	leech.damageModifierAddition = 10;
	leech.reflectModifierAddition = 1;
	leech.speedModifierAddition = 50.f;

	return leech;
}

SpellData SpellData::getIcyAmbushSpellData() {
	SpellData icyAmbush = EMPTY_SPELL;
	icyAmbush.id = SpellID::IcyAmbush;
	icyAmbush.spellType = SpellType::Twilight;
	icyAmbush.iconTextureRect = sf::IntRect(200, 50, 50, 50);

	icyAmbush.cooldown = sf::seconds(5);
	icyAmbush.boundingBox = sf::FloatRect(0, 0, 20, 20);
	icyAmbush.damageType = DamageType::Ice;
	icyAmbush.activeDuration = sf::seconds(5);
	icyAmbush.duration = sf::seconds(1);
	icyAmbush.needsTarget = true;
	icyAmbush.damage = 100;
	icyAmbush.startVelocity = 200.f;
	icyAmbush.range = 70.f;

	icyAmbush.damageModifierAddition = 50;
	icyAmbush.reflectModifierAddition = 1;
	icyAmbush.durationModifierAddition = sf::seconds(1);
	icyAmbush.rangeModifierAddition = 25.f;

	return icyAmbush;
}

SpellData SpellData::getFlashSpellData() {
	SpellData flash = EMPTY_SPELL;
	flash.id = SpellID::Flash;
	flash.spellType = SpellType::Twilight;
	flash.iconTextureRect = sf::IntRect(150, 50, 50, 50);

	flash.cooldown = sf::seconds(5);
	flash.damageType = DamageType::Light;
	flash.activeDuration = sf::seconds(2.0);
	flash.needsTarget = false;
	flash.damage = 20;
	flash.range = 150.f;
	flash.boundingBox = sf::FloatRect(0, 0, 100, 120);

	flash.damageModifierAddition = 20;
	flash.rangeModifierAddition = 100.f;

	return flash;
}

SpellData SpellData::getLightSpellData() {
	SpellData light = EMPTY_SPELL;
	light.id = SpellID::Light;
	light.spellType = SpellType::Divine;
	light.iconTextureRect = sf::IntRect(0, 150, 50, 50);

	light.cooldown = sf::seconds(60);
	light.boundingBox = sf::FloatRect(0, 0, 1, 1);
	light.duration = sf::seconds(60);
	light.activeDuration = light.duration;
	light.range = 200.f;

	light.durationModifierAddition = sf::seconds(60);
	light.rangeModifierAddition = 100.f;

	return light;
}

SpellData SpellData::getLeapOfFaithSpellData() {
	SpellData leapOfFaith = EMPTY_SPELL;
	leapOfFaith.id = SpellID::LeapOfFaith;
	leapOfFaith.spellType = SpellType::Divine;
	leapOfFaith.iconTextureRect = sf::IntRect(150, 150, 50, 50);

	leapOfFaith.cooldown = sf::seconds(30);
	leapOfFaith.boundingBox = sf::FloatRect(0, 0, 80, 120);
	leapOfFaith.duration = sf::seconds(10);
	leapOfFaith.activeDuration = leapOfFaith.duration;

	leapOfFaith.durationModifierAddition = sf::seconds(10);

	return leapOfFaith;
}

SpellData SpellData::getUnlockSpellData() {
	SpellData unlock = EMPTY_SPELL;
	unlock.id = SpellID::Unlock;
	unlock.spellType = SpellType::Twilight;
	unlock.iconTextureRect = sf::IntRect(0, 50, 50, 50);

	unlock.cooldown = sf::seconds(2);
	unlock.boundingBox = sf::FloatRect(0, 0, 10, 10);
	unlock.activeDuration = sf::seconds(1);
	unlock.needsTarget = true;
	unlock.startVelocity = 200.f;

	return unlock;
}

SpellData SpellData::getInvisibilitySpellData() {
	SpellData invisibility = EMPTY_SPELL;
	invisibility.id = SpellID::Invisibility;
	invisibility.spellType = SpellType::Twilight;
	invisibility.iconTextureRect = sf::IntRect(100, 50, 50, 50);

	invisibility.cooldown = sf::seconds(30);
	invisibility.boundingBox = sf::FloatRect(0, 0, 1, 1);
	invisibility.activeDuration = sf::seconds(5);
	invisibility.duration = invisibility.activeDuration;

	invisibility.durationModifierAddition = sf::seconds(5);

	return invisibility;
}




