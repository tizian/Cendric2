#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "TileMap.h"

class MapScreen;
class MapMainCharacter;
class MapOverlay;

class WaypointMarker : public AnimatedGameObject {
public:
	WaypointMarker(MapMainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent);

	void update(const sf::Time& frameTime) override;

	void onMouseOver() override;
	void onRightClick() override;
	void onLeftClick() override;
	void onInteractKey() override;

	GameObjectType getConfiguredType() const override;

private:
	MapMainCharacter* m_mainChar;
	MapOverlay* m_parent;
	bool m_isMouseOver = false;
	sf::Vector2f m_waypointPosition;
};

// the map overlay, as displayed in a map
// it takes its information from the corresponding map screen.
class MapOverlay {
public:
	MapOverlay(MapScreen* screen);
	~MapOverlay();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void reloadWaypoints();

	static float TOP;
	static float LEFT;

private:
	MapScreen* m_screen;

	TileMap m_backgroundTileMap;
	TileMap m_lightedForegroundTileMap;
	TileMap m_foregroundTileMap;

	sf::Sprite m_mainCharMarker;
	std::vector<WaypointMarker> m_waypoints;

	bool m_isVisible = false;

	float m_scale;
	sf::FloatRect m_boundingBox;
	sf::Vector2f m_position;
};