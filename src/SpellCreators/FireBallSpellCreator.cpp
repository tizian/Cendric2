#include "SpellCreators/FireBallSpellCreator.h"
#include "Screens/LevelScreen.h"

FireBallSpellCreator::FireBallSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void FireBallSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellData.count; i++) {
		FireBallSpell* newSpell = new FireBallSpell();
		spellData.divergenceAngle = div * sign * m_spellData.divergenceAngle;
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
		sign = -sign;
		if (sign == -1) {
			div += 1;
		}
	}

	m_owner->setFightAnimationTime();
}

void FireBallSpellCreator::addDamageModifier(int level) {
	m_spellData.damage += m_spellData.damageModifierAddition * level;
	m_spellData.damagePerSecond += static_cast<int>(std::floor(m_spellData.damageModifierAddition / m_spellData.duration.asSeconds())) * level;
}