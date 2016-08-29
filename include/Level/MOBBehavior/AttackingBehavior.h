#pragma once

#include "global.h"
#include "Level/LevelMovableGameObject.h"
#include "Level/LevelMainCharacter.h"

class Enemy;

// An enemy attacking behavior
class AttackingBehavior {
public:
	AttackingBehavior(LevelMovableGameObject* mob);
	virtual ~AttackingBehavior() {};

	virtual void update(const sf::Time& frameTime);
	
	virtual void handleAttackInput();

	void setAttackInput(const std::function<void()>& agent);

	static bool isInAggroRange(const LevelMainCharacter* mainChar, const Enemy* enemy, float aggroRange);

protected:
	LevelMovableGameObject* m_mob;
	LevelMainCharacter* m_mainChar;
	std::function<void()> m_executeAttack;
};