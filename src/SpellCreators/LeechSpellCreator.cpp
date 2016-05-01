#include "SpellCreators/LeechSpellCreator.h"
#include "Screens/LevelScreen.h"

LeechSpellCreator::LeechSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void LeechSpellCreator::execExecuteSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellData.count; i++) {
		LeechSpell* newSpell = new LeechSpell();
		spellData.divergenceAngle = div * sign * m_spellData.divergenceAngle;
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
		sign = -sign;
		if (sign == -1) {
			div += 1;
		}
	}
}