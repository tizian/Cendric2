#pragma once

#include "global.h"
#include "Weapon.h"

class IceStaff : public Weapon
{
public:
	~IceStaff();

	void load() override;

private:
};