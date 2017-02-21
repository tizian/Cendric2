#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "TileMap.h"
#include "BitmapText.h"

class WorldScreen;
class MainCharacter;
class MapOverlay;
class Window;

class WaypointMarker : public virtual AnimatedGameObject {
public:
	WaypointMarker(MainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void onMouseOver() override;
	void onRightClick() override;
	void onLeftClick() override;

	void setPosition(const sf::Vector2f& position) override;
	void loadAnimation();

	GameObjectType getConfiguredType() const override;

private:
	MainCharacter* m_mainChar = nullptr;
	MapOverlay* m_parent;
	bool m_isMouseOver = false;
	sf::Vector2f m_waypointPosition;
	BitmapText m_tooltip;
};

// the map overlay, as displayed in a map
// it takes its information from the corresponding world screen.
class MapOverlay {
public:
	MapOverlay(WorldScreen* screen);
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
	WorldScreen* m_screen;

	sf::Sprite m_map;
	TileMap m_fogOfWarTileMap;
	bool m_explorable = false;

	sf::Sprite m_mainCharMarker;
	std::vector<WaypointMarker*> m_waypoints;

	bool m_isVisible = false;

	float m_scale;
	sf::FloatRect m_boundingBox;
	sf::Vector2f m_position;

	BitmapText m_title;
	
	Window* m_window;
};