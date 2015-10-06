#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

// Animation of the Firebasket in the splashscreen
class FireBasket : public GameObject {
public:
	FireBasket();
	void load();

	GameObjectType getConfiguredType() const override;
};