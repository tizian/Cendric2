#include "Spells/SpellCreator.h"
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
	g_resourceManager->loadTexture(m_spellData.spritesheetPath, ResourceType::Level);
	g_resourceManager->loadSoundbuffer(m_spellData.soundPath, ResourceType::Level);
}

SpellCreator::~SpellCreator() {
}

void SpellCreator::update(const sf::Time& frametime) {
	if (m_currentCastingTime == sf::Time::Zero) return;

	m_currentCastingTime -= frametime;
	if (m_currentCastingTime <= sf::Time::Zero) {
		m_currentCastingTime = sf::Time::Zero;

		if (!m_owner->isDead()) {
			for (auto& target : m_futureTargets) {
				m_owner->setFacingRight(target.x - m_owner->getCenter().x > 0);
				execExecuteSpell(target);
			}
			if (m_target != nullptr) {
				m_owner->setFacingRight(m_target->getCenter().x - m_owner->getCenter().x > 0);
				execExecuteSpell(m_target->getCenter());
			}
			m_owner->executeFightAnimation(m_spellData.fightingTime, m_spellData.fightAnimation, m_spellData.isBlocking);
			m_isReady = true;
			m_futureTargets.clear();
			m_target = nullptr;
		}
	}
}

void SpellCreator::executeSpell(const sf::Vector2f& target) {
	m_isReady = false;
	m_owner->setFacingRight(target.x - m_owner->getCenter().x > 0);
	if (m_spellData.castingTime > sf::Time::Zero) {
		m_currentCastingTime = m_spellData.castingTime;
		m_futureTargets.push_back(target);
		m_owner->executeFightAnimation(m_spellData.castingTime, m_spellData.castingAnimation, m_spellData.isBlocking);
		return;
	}

	execExecuteSpell(target);
	m_owner->executeFightAnimation(m_spellData.fightingTime, m_spellData.fightAnimation, m_spellData.isBlocking);
	m_isReady = true;
}

void SpellCreator::executeSpell(const LevelMovableGameObject* target) {
	if (target == nullptr) return;
	m_isReady = false;
	m_owner->setFacingRight(target->getCenter().x - m_owner->getCenter().x > 0);
	if (m_spellData.castingTime > sf::Time::Zero) {
		m_currentCastingTime = m_spellData.castingTime;
		m_target = target;
		target->registerSpellCreator(this);
		m_owner->executeFightAnimation(m_spellData.castingTime, m_spellData.castingAnimation, m_spellData.isBlocking);
		return;
	}

	execExecuteSpell(target->getCenter());
	m_owner->executeFightAnimation(m_spellData.fightingTime, m_spellData.fightAnimation, m_spellData.isBlocking);
	m_isReady = true;
}

void SpellCreator::notifyMobDeath(LevelMovableGameObject* mob) {
	if (mob == m_target) {
		m_target = nullptr;
	}
}

void SpellCreator::addModifiers(const std::vector<SpellModifier>& modifiers) {
	for (auto& it : modifiers) {
		if (it.type == SpellModifierType::VOID) continue;
		// some security checks
		if (!contains(m_allowedModifiers, it.type)) {
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
	m_spellData.speed += m_spellData.speedModifierAddition * level;
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
	m_spellData.ccStrength += level;
}

SpellData& SpellCreator::getSpellData() {
	return m_spellData;
}

void SpellCreator::setSpellAllied(bool allied) {
	m_spellData.isAlly = allied;
}

void SpellCreator::updateDamageAndHeal(SpellData& bean) const {
	updateDamageAndHeal(bean, m_attributeData, true);
}

bool SpellCreator::isReady() const {
	return m_isReady;
}

int SpellCreator::getStrengthModifierValue() const {
	return m_spellData.ccStrength;
}

std::string SpellCreator::getStrengthModifierName() const {
	return "";
}

void SpellCreator::updateDamageAndHeal(SpellData& bean, const AttributeData* attributes, bool includeRngAndCrit) {
	if (attributes == nullptr) return;

	// handle heal
	if (bean.heal > 0) {
		bean.heal += attributes->heal;
	}

	// handle instant damage
	if (bean.damage > 0) {
		switch (bean.damageType) {
		case DamageType::Physical:
			bean.damage += attributes->damagePhysical;
			break;
		case DamageType::Fire:
			bean.damage += attributes->damageFire;
			break;
		case DamageType::Ice:
			bean.damage += attributes->damageIce;
			break;
		case DamageType::Shadow:
			bean.damage += attributes->damageShadow;
			break;
		case DamageType::Light:
			bean.damage += attributes->damageLight;
			break;
		default:
			break;
		}
	}

	// handle DOT damage
	if (bean.damagePerSecond > 0 && bean.duration.asSeconds() > 0.f) {
		float durationS = bean.duration.asSeconds();
		switch (bean.damageType) {
		case DamageType::Physical:
			bean.damagePerSecond += static_cast<int>(attributes->damagePhysical / durationS);
			break;
		case DamageType::Fire:
			bean.damagePerSecond += static_cast<int>(attributes->damageIce / durationS);
			break;
		case DamageType::Ice:
			bean.damagePerSecond += static_cast<int>(attributes->damageFire / durationS);
			break;
		case DamageType::Shadow:
			bean.damagePerSecond += static_cast<int>(attributes->damageShadow / durationS);
			break;
		case DamageType::Light:
			bean.damagePerSecond += static_cast<int>(attributes->damageLight / durationS);
			break;
		default:
			break;
		}
	}

	if (!includeRngAndCrit) return;

	// add randomness to damage (something from 80 - 120% of the base damage)
	bean.damage = static_cast<int>(bean.damage * ((rand() % 41 + 80.f) / 100.f));
	// add randomness to heal (something from 80 - 120% of the base heal)
	bean.heal = static_cast<int>(bean.heal * ((rand() % 41 + 80.f) / 100.f));

	// add critical hit to damage
	int chance = rand() % 100 + 1;
	if (bean.damage > 0 && chance <= attributes->criticalHitChance) {
		bean.critical = true;
		bean.damage *= 2;
	}
	// add critical hit to heal
	chance = rand() % 100 + 1;
	if (bean.heal > 0 && chance <= attributes->criticalHitChance) {
		bean.critical = true;
		bean.heal *= 2;
	}
}