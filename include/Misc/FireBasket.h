#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"

// Animation of the Firebasket in the splashscreen
class FireBasket : public AnimatedGameObject {
public:
	FireBasket();
	void load();

	GameObjectType getConfiguredType() const override;
};