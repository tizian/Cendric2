#include "Structs/SpellBean.h"
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

std::vector<SpellModifierType> SpellBean::getAllowedModifiers(SpellID id)
{
	std::vector<SpellModifierType> types;
	switch (id)
	{
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
	default:
		break;
	}
	return types;
}

SpellCreator* SpellBean::getSpellCreator(const SpellBean& bean, const std::vector<SpellModifier>& modifiers, LevelMovableGameObject* owner)
{
	SpellCreator* creator;
	switch (bean.id)
	{
	case SpellID::Chop:
		creator = new ChopSpellCreator(bean, owner);
		break;
	case SpellID::FireBall:
		creator = new FireBallSpellCreator(bean, owner);
		break;
	case SpellID::IceBall:
		creator = new IceBallSpellCreator(bean, owner);
		break;
	case SpellID::DivineShield:
		creator = new DivineShieldSpellCreator(bean, owner);
		break;
	case SpellID::Aureola:
		creator = new AureolaSpellCreator(bean, owner);
		break;
	case SpellID::Fear:
		creator = new FearSpellCreator(bean, owner);
		break;
	case SpellID::AntiGravity:
		creator = new AntiGravitySpellCreator(bean, owner);
		break;
	case SpellID::WindGust:
		creator = new WindGustSpellCreator(bean, owner);
		break;
	case SpellID::Leech:
		creator = new LeechSpellCreator(bean, owner);
		break;
	case SpellID::IcyAmbush:
		creator = new IcyAmbushSpellCreator(bean, owner);
		break;
	case SpellID::Unlock:
		creator = new UnlockSpellCreator(bean, owner);
		break;
	default:
		return nullptr;
	}

	creator->addModifiers(modifiers);

	return creator;
}

SpellBean SpellBean::getSpellBean(SpellID id)
{
	switch (id)
	{
	case SpellID::Chop:
		return getChopSpellBean();
	case SpellID::FireBall:
		return getFireBallSpellBean();
	case SpellID::IceBall:
		return getIceBallSpellBean();
	case SpellID::Aureola:
		return getAureolaSpellBean();
	case SpellID::DivineShield:
		return getDivineShieldSpellBean();
	case SpellID::Fear:
		return getFearSpellBean();
	case SpellID::AntiGravity:
		return getAntiGravitySpellBean();
	case SpellID::WindGust:
		return getWindGustSpellBean();
	case SpellID::Leech:
		return getLeechSpellBean();
	case SpellID::IcyAmbush:
		return getIcyAmbushSpellBean();
	case SpellID::Unlock:
		return getUnlockSpellBean();
	default:
		return EMPTY_SPELL;
	}
}

SpellBean SpellBean::getChopSpellBean()
{
	SpellBean chop = EMPTY_SPELL;
	chop.id = SpellID::Chop;

	chop.cooldown = sf::milliseconds(400);
	chop.boundingBox = sf::FloatRect(0, 0, 40, 80);
	chop.damageType = DamageType::Physical;
	chop.damage = 3;
	chop.duration = sf::milliseconds(320);

	chop.inputKey = Key::Chop;
	
	return chop;
}

SpellBean SpellBean::getFireBallSpellBean()
{
	SpellBean fireBall = EMPTY_SPELL;
	fireBall.id = SpellID::FireBall;
	fireBall.spellType = SpellType::Elemental;
	fireBall.iconTextureRect = sf::IntRect(0, 0, 50, 50);

	fireBall.cooldown = sf::seconds(1);
	fireBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fireBall.divergenceAngle = 0.2f;
	fireBall.damageType = DamageType::Fire;
	fireBall.duration = sf::seconds(5);
	fireBall.needsTarget = true;
	fireBall.damage = 10;
	fireBall.startVelocity = 300.f;

	fireBall.countModifierAddition = 1;
	fireBall.damageModifierAddition = 20;
	fireBall.reflectModifierAddition = 1;
	fireBall.speedModifierAddition = 100.f;

	return fireBall;
}

SpellBean SpellBean::getIceBallSpellBean()
{
	SpellBean iceBall = EMPTY_SPELL;
	iceBall.id = SpellID::IceBall;
	iceBall.spellType = SpellType::Elemental;
	iceBall.iconTextureRect = sf::IntRect(50, 0, 50, 50);

	iceBall.cooldown = sf::seconds(1);
	iceBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	iceBall.divergenceAngle = 0.2f;
	iceBall.damageType = DamageType::Ice;
	iceBall.duration = sf::seconds(5);
	iceBall.needsTarget = true;
	iceBall.damage = 6;
	iceBall.startVelocity = 200.f;

	iceBall.countModifierAddition = 1;
	iceBall.damageModifierAddition = 20;
	iceBall.reflectModifierAddition = 1;
	iceBall.speedModifierAddition = 100.f;
	
	return iceBall;
}

SpellBean SpellBean::getDivineShieldSpellBean()
{
	SpellBean divineShield = EMPTY_SPELL;
	divineShield.id = SpellID::DivineShield;
	divineShield.spellType = SpellType::Divine;
	divineShield.iconTextureRect = sf::IntRect(100, 150, 50, 50);

	divineShield.cooldown = sf::milliseconds(10000);
	divineShield.boundingBox = sf::FloatRect(0, 0, 98, 98);
	divineShield.duration = sf::milliseconds(3000);
	divineShield.heal = 20;

	divineShield.durationModifierAddition = sf::milliseconds(2000);

	return divineShield;
}


SpellBean SpellBean::getAureolaSpellBean()
{
	SpellBean aureola = EMPTY_SPELL;
	aureola.id = SpellID::Aureola;
	aureola.spellType = SpellType::Divine;
	aureola.iconTextureRect = sf::IntRect(200, 150, 50, 50);

	aureola.cooldown = sf::milliseconds(10000);
	aureola.boundingBox = sf::FloatRect(0, 0, 20, 20);
	aureola.count = 4,
	aureola.divergenceAngle = 2 * M_PI / aureola.count;
	aureola.damageType = DamageType::Light;
	aureola.duration = sf::milliseconds(10000);
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

SpellBean SpellBean::getFearSpellBean()
{
	SpellBean fear = EMPTY_SPELL;
	fear.id = SpellID::Fear;
	fear.spellType = SpellType::Necromancy;
	fear.iconTextureRect = sf::IntRect(0, 100, 50, 50);

	fear.cooldown = sf::seconds(3);
	fear.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fear.divergenceAngle = 0.2f;
	fear.duration = sf::seconds(5);
	fear.needsTarget = true;
	fear.startVelocity = 300.f;

	fear.countModifierAddition = 1;
	fear.reflectModifierAddition = 1;
	fear.speedModifierAddition = 100.f;
	fear.durationModifierAddition = sf::seconds(2);

	return fear;
}

SpellBean SpellBean::getAntiGravitySpellBean()
{
	SpellBean antiGravity = EMPTY_SPELL;
	antiGravity.id = SpellID::AntiGravity;
	antiGravity.spellType = SpellType::Elemental;
	antiGravity.iconTextureRect = sf::IntRect(200, 0, 50, 50);

	antiGravity.cooldown = sf::seconds(15);
	antiGravity.boundingBox = sf::FloatRect(0, 0, 98, 98);
	antiGravity.duration = sf::seconds(5);

	antiGravity.durationModifierAddition = sf::seconds(3);

	return antiGravity;
}

SpellBean SpellBean::getWindGustSpellBean()
{
	SpellBean windGust = EMPTY_SPELL;
	windGust.id = SpellID::WindGust;
	windGust.spellType = SpellType::Elemental;

	windGust.iconTextureRect = sf::IntRect(150, 0, 50, 50);
	windGust.cooldown = sf::seconds(3);
	windGust.boundingBox = sf::FloatRect(0, 0, 60, 40);
	windGust.duration = sf::seconds(1);

	windGust.rangeModifierAddition = 20.f;
	windGust.durationModifierAddition = sf::seconds(1);

	return windGust;
}

SpellBean SpellBean::getLeechSpellBean()
{
	SpellBean leech = EMPTY_SPELL;
	leech.id = SpellID::Leech;
	leech.spellType = SpellType::Necromancy;
	leech.iconTextureRect = sf::IntRect(50, 100, 50, 50);

	leech.cooldown = sf::seconds(3);
	leech.boundingBox = sf::FloatRect(0, 0, 10, 10);
	leech.divergenceAngle = 0.2f;
	leech.damageType = DamageType::Shadow;
	leech.duration = sf::seconds(5);
	leech.needsTarget = true;
	leech.damage = 10;
	leech.startVelocity = 150.f;

	leech.countModifierAddition = 1;
	leech.damageModifierAddition = 10;
	leech.reflectModifierAddition = 1;
	leech.speedModifierAddition = 50.f;

	return leech;
}

SpellBean SpellBean::getIcyAmbushSpellBean()
{
	SpellBean icyAmbush = EMPTY_SPELL;
	icyAmbush.id = SpellID::IcyAmbush;
	icyAmbush.spellType = SpellType::Twilight;
	icyAmbush.iconTextureRect = sf::IntRect(200, 50, 50, 50);

	icyAmbush.cooldown = sf::seconds(5);
	icyAmbush.boundingBox = sf::FloatRect(0, 0, 20, 20);
	icyAmbush.damageType = DamageType::Ice;
	icyAmbush.duration = sf::seconds(5);
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

SpellBean SpellBean::getUnlockSpellBean()
{
	SpellBean unlock = EMPTY_SPELL;
	unlock.id = SpellID::Unlock;
	unlock.spellType = SpellType::Twilight;
	unlock.iconTextureRect = sf::IntRect(0, 50, 50, 50);

	unlock.cooldown = sf::seconds(2);
	unlock.boundingBox = sf::FloatRect(0, 0, 10, 10);
	unlock.duration = sf::seconds(1);
	unlock.needsTarget = true;
	unlock.startVelocity = 200.f;

	return unlock;
}




