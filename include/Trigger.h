#pragma once

#include "global.h"
#include "GameObject.h"
#include "Structs/TriggerData.h"

class WorldScreen;

// A trigger, on a map or a level, waiting to get triggered.
// level and map exits are triggers too.
class Trigger : public GameObject {
public:
	Trigger(WorldScreen* screen, const TriggerData& data);
	~Trigger() {};

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	TriggerData& getData();

	GameObjectType getConfiguredType() const override;

private:
	GameObject* m_mainChar;
	WorldScreen* m_worldScreen;
	bool m_isOnTrigger = true;
	
	TriggerData m_data;

	// Arrow information
	bool m_showSprite = false;
	sf::Sprite m_sprite;
	sf::Time m_time;
};