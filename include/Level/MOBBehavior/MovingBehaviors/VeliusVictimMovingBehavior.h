#pragma once

#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

class VeliusVictim;

// A moving behavior for the three victims of Velius
class VeliusVictimMovingBehavior final : public WalkingBehavior {
public:
	VeliusVictimMovingBehavior(VeliusVictim* enemy);
	~VeliusVictimMovingBehavior() {};

	void callToDie();
	void release();
	void execHandleMovementInput() override;
	void updateAnimation(const sf::Time& frameTime) override;

private:
	VeliusVictim* m_victim;
	static const sf::Vector2f ALTAR_POS;
	sf::Vector2f m_initialPos;
	bool m_isReturning;
};