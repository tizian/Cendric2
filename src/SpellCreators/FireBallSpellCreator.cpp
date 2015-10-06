#include "SpellCreators/FireBallSpellCreator.h"
#include "Screens/LevelScreen.h"

FireBallSpellCreator::FireBallSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner) {
}

void FireBallSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellBean.count; i++) {
		FireBallSpell* newSpell = new FireBallSpell();
		spellBean.divergenceAngle = div * sign * m_spellBean.divergenceAngle;
		newSpell->load(spellBean, m_owner, target);
		m_screen->addObject(newSpell);
		sign = -sign;
		if (sign == -1) {
			div += 1;
		}
	}

	m_owner->setFightAnimationTime();
}