#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

class VeliusBoss;

// A moving behavior for the mob "Velius Boss"
class VeliusBossMovingBehavior final : public WalkingBehavior {
public:
	VeliusBossMovingBehavior(VeliusBoss* enemy);
	~VeliusBossMovingBehavior() {};

	void execHandleMovementInput() override;
	void updateAnimation(const sf::Time& frameTime) override;

private:
	VeliusBoss* m_boss;
};