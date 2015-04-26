#pragma once

#include "global.h"
#include "Weapon.h"

class IceStaff : public Weapon
{
public:
	IceStaff();
	~IceStaff();

	void load() override;

private:
};