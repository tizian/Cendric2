#pragma once

#include "global.h"
#include "GameObject.h"
#include "Structs/TriggerData.h"

class WorldScreen;

// A trigger, on a map or a level, waiting to get triggered.
class Trigger : public GameObject {
public:
	Trigger(WorldScreen* screen, GameObject* mainCharacter, const TriggerData& data);
	~Trigger() {};

	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;

private:
	GameObject* m_mainChar;
	WorldScreen* m_worldScreen;
	
	TriggerData m_data;
};