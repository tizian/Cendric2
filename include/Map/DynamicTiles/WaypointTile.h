#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "LightObject.h"
#include "GUI/BitmapText.h"

class WaypointTile : public MapDynamicTile {
public:
	WaypointTile(Map* map);
	void init() override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frameTime) override;
	void onMouseOver() override;
	void onInteractKey() override;
	void onRightClick() override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	void setActive();
	void setSpawnPosition(int spawnPosition);
	void setPosition(const sf::Vector2f& pos) override;
	void setDisposed() override;
	void setScreen(Screen* screen) override;

private:
	LightObject* m_lightObject;
	MapMainCharacter* m_mainCharacter;
	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	int m_spawnPosition = -1;
};