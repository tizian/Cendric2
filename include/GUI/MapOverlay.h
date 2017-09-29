#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "World/TileMap.h"
#include "BitmapText.h"

class WorldScreen;
class MainCharacter;
class MapOverlay;
class Window;
class GUITabBar;

struct MapOverlayData final {
	bool isLevel;
	std::string mapId;
	sf::FloatRect windowSize;
	sf::Vector2i mapSize;
	float scale;
	sf::Sprite map;
	TileMap fogOfWarTileMap;
};

class WaypointMarker final : public AnimatedGameObject {
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
	MapOverlay(WorldScreen* screen, GUITabBar* mapTabBar);
	~MapOverlay();

	void show();
	void hide();

	bool isVisible() const;
	bool isOnCurrentMap() const;
	MapOverlayData* getCurrentMap() const;
	WorldScreen* getScreen() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void reloadWaypoints();
	void reloadMaps();
	void notifyLevelOverlayReload();

	void setMap(const std::string& mapID);

	static std::string getMapSpriteFilename(const std::string& mapID);
	static std::string getMapIconFilename(const std::string& mapID);

private:
	void setMapIndex(int index);
	void updateFogOfWar(MapOverlayData* map);
	MapOverlayData* createMapOverlayData(const std::string& id, const sf::Vector2i& size, const sf::Sprite& sprite) const;
	sf::Sprite* renderLevelOverlay(float scale);
	void drawOverlayTexture(sf::Image& image, const sf::Vector2f& pos, int type);
	float getScale(const sf::Vector2f& mapSize) const;
	void reloadLevelOverlay();

private:
	static const float TOP;
	static const float LEFT;
	static const float MAX_HEIGHT;
	static const float MAX_WIDTH;

private:
	WorldScreen* m_screen;

	std::vector<MapOverlayData*> m_maps;

	sf::Sprite m_mainCharMarker;
	sf::Image m_levelOverlayIcons;
	std::vector<WaypointMarker*> m_waypoints;

	int m_currentMap = -1;
	bool m_isVisible = false;
	bool m_isOnCurrentMap = false;
	bool m_isLevel = false;
	bool m_needsLevelOverlayReload = false;

	sf::FloatRect m_boundingBox;
	sf::Vector2f m_position;

	BitmapText m_title;
	
	Window* m_window;
	GUITabBar* m_mapTabBar;
};