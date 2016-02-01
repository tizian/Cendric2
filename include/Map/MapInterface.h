#pragma once

#include "global.h"
#include "GUI/GameInterface.h"

#include "GUI/MapOverlay.h"

// The interface overlay in a map
// if the keys for character screen, spellbook and inventory are pressed, these will display.
class MapInterface : public GameInterface {
public:
	MapInterface(GameScreen* screen);
	~MapInterface();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

private:
	void updateMapOverlay(const sf::Time& frameTime);

	MapOverlay* m_mapOverlay;
};