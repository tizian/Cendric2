#pragma once

#include "global.h"
#include "World/TileMap.h"
#include "BitmapText.h"
#include "Structs/QuestData.h"
#include "GUI/QuestMarker.h"
#include "GUI/SelectableWindow.h"
#include "GUI/ButtonInterface.h"
#include "GUI/JoystickButtonGroup.h"

class WorldScreen;
class WorldInterface;
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

class WaypointMarker final : public AnimatedGameObject, public ButtonInterface {
public:
	WaypointMarker(MainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void onLeftClick() override;
	void onMouseOver() override;

	void setPosition(const sf::Vector2f& position) override;
	void loadAnimation();

	void click() override;

	void setState(GameObjectState state) override { AnimatedGameObject::setState(state); }
	GameObjectType getConfiguredType() const override { return ButtonInterface::getConfiguredType(); }

protected:
	void updateColor() override;
	std::string getTooltipString() const;

private:
	MainCharacter* m_mainChar = nullptr;
	MapOverlay* m_parent;
	sf::Vector2f m_waypointPosition;
	BitmapText m_tooltip;
};

// a quest marker on the map
class MapQuestMarker final : public QuestMarker, public ButtonInterface {
public:
	MapQuestMarker(const QuestData& questData, const QuestMarkerData& markerData, WorldInterface* interface);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override { QuestMarker::render(target); }
	void renderAfterForeground(sf::RenderTarget& target) override { QuestMarker::renderAfterForeground(target); }
	void setPosition(const sf::Vector2f& position) override { QuestMarker::setPosition(position); }

	void onLeftClick() override;
	void onMouseOver() override;

	void setTooltipTime(const sf::Time& time);
	const std::string& getQuestId();
	void click() override;

	GameObjectType getConfiguredType() const override { return ButtonInterface::getConfiguredType(); }

protected:
	void updateColor() override;

private:
	void init();
	void jumpToQuest();
	
	QuestMarkerData m_markerData;
	WorldInterface* m_interface;
};

// the map overlay, as displayed in a map
// it takes its information from the corresponding world screen.
class MapOverlay final : public SelectableWindow {
public:
	MapOverlay(WorldInterface* interface, GUITabBar* mapTabBar);
	~MapOverlay();

	void show();
	void hide();

	bool isVisible() const;
	bool isOnCurrentMap() const;
	MapOverlayData* getCurrentMap() const;
	WorldScreen* getScreen() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void reloadMaps();
	void reloadMarkers();

	void notifyLevelOverlayReload();
	void notifyJumpToQuest(const std::string& questId, const std::vector<QuestMarkerData>& data);

	// return whether map has been set
	bool setMap(const std::string& mapID);

	static std::string getMapSpriteFilename(const std::string& mapID);
	static std::string getMapIconFilename(const std::string& mapID);

private:
	void reloadWaypoints();
	void reloadQuestMarkers();

	void setMapIndex(int index);
	void updateFogOfWar(MapOverlayData* map);
	MapOverlayData* createMapOverlayData(const std::string& id, const sf::Vector2i& size, const sf::Sprite& sprite) const;
	void renderLevelOverlay(float scale);
	void drawOverlayTexture(sf::Image& image, const sf::Vector2f& pos, int posX, int posY);
	float getScale(const sf::Vector2f& mapSize) const;
	void reloadLevelOverlay();
	void reloadButtonGroup();

protected:
	void updateWindowSelected() override;
	void updateSelectableWindow();

private:
	static const float TOP;
	static const float LEFT;
	static const float MAX_HEIGHT;
	static const float MAX_WIDTH;

private:
	WorldScreen* m_screen;
	WorldInterface* m_interface;

	std::vector<MapOverlayData*> m_maps;

	sf::Sprite m_mainCharMarker;
	sf::Image m_levelOverlayIcons;
	sf::Sprite m_levelOverlaySprite;
	sf::Texture m_levelOverlayTexture;

	std::vector<WaypointMarker*> m_waypoints;
	std::vector<MapQuestMarker*> m_questMarkers;
	JoystickButtonGroup* m_buttonGroup = nullptr;

	int m_currentMap = -1;
	bool m_isVisible = false;
	bool m_isOnCurrentMap = false;
	bool m_isLevel = false;
	bool m_needsLevelOverlayReload = false;

	sf::FloatRect m_boundingBox;
	sf::Vector2f m_position;

	BitmapText m_title;

	GUITabBar* m_mapTabBar;
};