#pragma once

#include "global.h"
#include "GUI/WorldInterface.h"

#include "GUI/MapOverlay.h"

// The interface overlay in a map
// if the keys for character screen, spellbook and inventory are pressed, these will display.
class MapInterface : public WorldInterface {
public:
	MapInterface(WorldScreen* screen);
	~MapInterface();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	bool isMapOverlayVisible() const;

private:
	void updateMapOverlay(const sf::Time& frameTime);

	MapOverlay* m_mapOverlay;
};