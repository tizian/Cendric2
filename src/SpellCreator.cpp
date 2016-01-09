#include "SpellCreator.h"
#include "Screens/LevelScreen.h"

SpellCreator::SpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) {
	m_owner = owner;
	if (m_owner != nullptr) m_level = owner->getLevel();
	if (m_owner != nullptr) m_attributeData = owner->getAttributes();
	if (m_owner != nullptr && !(m_screen = dynamic_cast<LevelScreen*>(owner->getScreen()))) {
		g_logger->logError("SpellCreator", "spell owner has no (level)screen.");
	}
	m_spellData = spellData;

	m_allowedModifiers = SpellData::getAllowedModifiers(spellData.id);
}

SpellCreator::~SpellCreator() {
}

void SpellCreator::addModifiers(const std::vector<SpellModifier>& modifiers) {
	for (auto& it : modifiers) {
		if (it.type == SpellModifierType::VOID) continue;
		// some security checks
		if (!(std::find(m_allowedModifiers.begin(), m_allowedModifiers.end(), it.type) != m_allowedModifiers.end())) {
			g_logger->logWarning("SpellCreator", "Modifier of an unallowed type was ignored.");
			continue;
		}
		if (it.level < 1 || it.level > 3) {
			g_logger->logWarning("SpellCreator", "Modifier of an unallowed level was ignored.");
			continue;
		}
		switch (it.type) {
		case SpellModifierType::Count:
			addCountModifier(it.level);
			break;
		case SpellModifierType::Damage:
			addDamageModifier(it.level);
			break;
		case SpellModifierType::Range:
			addRangeModifier(it.level);
			break;
		case SpellModifierType::Reflect:
			addReflectModifier(it.level);
			break;
		case SpellModifierType::Speed:
			addSpeedModifier(it.level);
			break;
		case SpellModifierType::Strength:
			addStrengthModifier(it.level);
			break;
		case SpellModifierType::Duration:
			addDurationModifier(it.level);
			break;
		default:
			g_logger->logWarning("SpellCreator", "Modifier of an unknown type was ignored.");
			break;
		}
	}
}

void SpellCreator::addSpeedModifier(int level) {
	m_spellData.startVelocity += m_spellData.speedModifierAddition * level;
}

void SpellCreator::addDamageModifier(int level) {
	m_spellData.damage += m_spellData.damageModifierAddition * level;
}

void SpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}

void SpellCreator::addRangeModifier(int level) {
	m_spellData.range += m_spellData.rangeModifierAddition * level;
}

void SpellCreator::addCountModifier(int level) {
	m_spellData.count += m_spellData.countModifierAddition * level;
}

void SpellCreator::addReflectModifier(int level) {
	m_spellData.reflectCount += m_spellData.reflectModifierAddition * level;
}

void SpellCreator::addStrengthModifier(int level) {
	// nop, we don't know yet what this does but the method is implemented so its subclasses won't have to
}

const SpellData& SpellCreator::getSpellData() const {
	return m_spellData;
}

void SpellCreator::updateDamage(SpellData& bean) const {
	updateDamage(bean, m_attributeData, true);
}

int SpellCreator::getStrengthModifierValue() const {
	return 0;
}

std::string SpellCreator::getStrengthModifierName() const {
	return "";
}

void SpellCreator::updateDamage(SpellData& bean, const AttributeData* attributes, bool includeRngAndCrit) {
	if (attributes == nullptr) return;
	if (bean.damage > 0) {
		switch (bean.damageType) {
		case DamageType::Physical:
			bean.damage = bean.damage + attributes->damagePhysical;
			break;
		case DamageType::Fire:
			bean.damage = bean.damage + attributes->damageFire;
			break;
		case DamageType::Ice:
			bean.damage = bean.damage + attributes->damageIce;
			break;
		case DamageType::Shadow:
			bean.damage = bean.damage + attributes->damageShadow;
			break;
		case DamageType::Light:
			bean.damage = bean.damage + attributes->damageLight;
			break;
		default:
			break;
		}
	}
	if (bean.damagePerSecond > 0 && bean.duration.asSeconds() > 0.f) {
		float durationS = bean.duration.asSeconds();
		switch (bean.damageType) {
		case DamageType::Physical:
			bean.damagePerSecond = bean.damagePerSecond + static_cast<int>(attributes->damagePhysical / durationS);
			break;
		case DamageType::Fire:
			bean.damagePerSecond = bean.damagePerSecond + static_cast<int>(attributes->damageIce / durationS);
			break;
		case DamageType::Ice:
			bean.damagePerSecond = bean.damagePerSecond + static_cast<int>(attributes->damageFire / durationS);
			break;
		case DamageType::Shadow:
			bean.damagePerSecond = bean.damagePerSecond + static_cast<int>(attributes->damageShadow / durationS);
			break;
		case DamageType::Light:
			bean.damagePerSecond = bean.damagePerSecond + static_cast<int>(attributes->damageLight / durationS);
			break;
		default:
			break;
		}
	}

	if (!includeRngAndCrit) return;

	// add randomness to damage (something from 80 - 120% of the base damage)
	bean.damage = static_cast<int>(bean.damage * ((rand() % 41 + 80.f) / 100.f));

	// add critical hit to damage
	int chance = rand() % 100 + 1;
	if (chance <= attributes->criticalHitChance) {
		bean.damage *= 2;
	}
}