#pragma once

#include "global.h"

class LevelMainCharacter;
class Enemy;
class LevelScreen;

class TargetManager {
public:
	TargetManager();
	~TargetManager();

	void update(const sf::Time& frameTime);

	void setTargetEnemy(Enemy* enemy);
	void setMainCharacter(LevelMainCharacter* mainChar);
	Enemy* getCurrentTargetEnemy() const;
	
private:
	LevelMainCharacter* m_mainChar;
	LevelScreen* m_screen;

	Enemy* m_targetedEnemy = nullptr;
	std::set<Enemy*> m_previousTargets;

	static const float TARGET_RANGE;
};