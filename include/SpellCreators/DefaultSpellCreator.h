#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"
#include "Screens/LevelScreen.h"

// a class that creates all kinds of simple spells that need no special behaviour
template <class T>
class DefaultSpellCreator : public SpellCreator {
public:
	DefaultSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
		static_assert(std::is_base_of<Spell, T>::value, "type parameter of DefaultSpellCreator must derive from Spell");
	}

	void execExecuteSpell(const sf::Vector2f& target) override {
		SpellData spellData = m_spellData;
		updateDamageAndHeal(spellData);
		int div = 0;
		int sign = 1;
		for (int i = 0; i < m_spellData.count; i++) {
			T* newSpell = new T();
			spellData.divergenceAngle = div * sign * m_spellData.divergenceAngle;
			newSpell->load(spellData, m_owner, target);
			m_screen->addObject(newSpell);
			sign = -sign;
			if (sign == -1) {
				div += 1;
			}
		}
	}
};