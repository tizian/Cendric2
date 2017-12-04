#pragma once

#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

class VeliusVictim;

// A moving behavior for the three victims of Velius
class VeliusVictimMovingBehavior final : public WalkingBehavior {
public:
	VeliusVictimMovingBehavior(VeliusVictim* enemy);
	~VeliusVictimMovingBehavior() {};

	void callToDie();
	void execHandleMovementInput() override;

private:
	VeliusVictim* m_victim;
	bool m_isCalled = false;
	bool m_isAtAltar = false;
	static const sf::Vector2f ALTAR_POS;
};