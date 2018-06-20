#include "GUI/MapOverlay.h"
#include "GUI/Window.h"
#include "GUI/GUITabBar.h"
#include "GUI/GUIConstants.h"
#include "Screens/WorldScreen.h"
#include "Screens/LevelScreen.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/NPC.h"
#include "Level/DynamicTiles/ChestLevelTile.h"
#include "World/MainCharacter.h"
#include "World/Trigger.h"
#include "GlobalResource.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "Structs/LevelData.h"
#include "Controller/GamepadMappings.h"

const float MapOverlay::TOP = 30.f;
const float MapOverlay::LEFT = GUIConstants::LEFT;
const float MapOverlay::MAX_WIDTH = WINDOW_WIDTH - 2 * LEFT;
const float MapOverlay::MAX_HEIGHT = WINDOW_HEIGHT - 2 * TOP;

MapOverlay::MapOverlay(WorldInterface* interface, GUITabBar* mapTabBar) {
	m_interface = interface;
	m_screen = interface->getScreen();
	m_mapTabBar = mapTabBar;

	m_levelOverlayIcons.loadFromFile(getResourcePath(GlobalResource::TEX_GUI_LEVELOVERLAY_ICONS));

	const World& map = *m_screen->getWorld();

	m_mainCharMarker.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_LEVELOVERLAY_ICONS));
	m_mainCharMarker.setTextureRect(sf::IntRect(0, 25, 25, 25));

	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_title.setTextStyle(TextStyle::Shadowed);

	m_window = new Window(sf::FloatRect(),
		GUIOrnamentStyle::LARGE,
		COLOR_TRANSPARENT,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&MapOverlay::hide, this));

	reloadMaps();

	const std::string mapName = m_screen->getCharacterCore()->getData().lastOverworldMap;

	setMap(mapName);
	if (m_isLevel || m_currentMap == -1) {
		setMapIndex(0);
	}
}

MapOverlay::~MapOverlay() {
	CLEAR_VECTOR(m_waypoints);
	CLEAR_VECTOR(m_questMarkers);
	CLEAR_VECTOR(m_maps);

	delete m_window;
	delete m_buttonGroup;
}

void MapOverlay::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;
	updateSelectableWindow();
	auto* map = getCurrentMap();
	if (map == nullptr) return;

	if (m_needsLevelOverlayReload) {
		reloadLevelOverlay();
	}

	if (m_isOnCurrentMap) {
		updateFogOfWar(map);
		m_mainCharMarker.setPosition(m_position +
			m_screen->getMainCharacter()->getCenter() * map->scale - sf::Vector2f(12.5f, 12.5f));
	}

	for (auto qm : m_questMarkers) {
		qm->update(frameTime);
	}

	for (auto wp : m_waypoints) {
		wp->update(frameTime);
	}

	m_buttonGroup->update(frameTime);

	for (int i = 0; i < static_cast<int>(m_buttonGroup->getButtons().size()); ++i) {
		if (m_buttonGroup->getButtons()[i]->isMousedOver()) {
			m_buttonGroup->selectButton(i);
			break;
		}
	}

	m_window->update(frameTime);
}

void MapOverlay::updateSelectableWindow() {
	if (!isWindowSelected() || g_inputController->isActionLocked()) {
		return;
	}

	if (!m_buttonGroup->getButtons().empty()) {
		return;
	}

	if (g_inputController->isJustLeft()) {
		setLeftWindowSelected();
	}

	if (g_inputController->isJustRight()) {
		setRightWindowSelected();
	}
}

bool MapOverlay::setMap(const std::string& mapID) {
	int index = 0;

	for (auto& it : m_maps) {
		if (it->mapId == mapID) {
			setMapIndex(index);
			return true;
		}
		index++;
	}

	return false;
}

void MapOverlay::setMapIndex(int index) {
	if (index < 0 || index > static_cast<int>(m_maps.size()) - 1) return;

	auto const& map = m_maps[index];

	m_boundingBox = map->windowSize;
	m_position.x = m_boundingBox.left;
	m_position.y = m_boundingBox.top;

	m_window->setSize(sf::Vector2f(m_boundingBox.width + 1.f, m_boundingBox.height + 1.f));
	m_window->setPosition(sf::Vector2f(m_position.x - 1.f, m_position.y - 1.f));

	map->map.setPosition(m_position);
	map->fogOfWarTileMap.setPosition(m_position);
	updateFogOfWar(map);

	auto worldName = g_textProvider->getText(World::getNameFromId(map->mapId), "location");
	auto breakPos = worldName.find('\n');
	while (breakPos != std::string::npos) {
		worldName.replace(breakPos, 1, " ");
		breakPos = worldName.find('\n');
	}
	m_title.setString(worldName);
	m_title.setPosition(sf::Vector2f((WINDOW_WIDTH - m_title.getBounds().width) / 2.f, m_boundingBox.top - 24.f));

	m_isOnCurrentMap = (m_screen->getWorldData()->id == map->mapId);
	m_currentMap = index;

	reloadMarkers();
}

void MapOverlay::updateFogOfWar(MapOverlayData* map) {
	if (map->isLevel) return;
	if (!m_screen->getCharacterCore()->isMapExplored(map->mapId)) return;
	auto const& currentMap = m_screen->getCharacterCore()->getExploredTiles().at(map->mapId);
	map->fogOfWarTileMap.updateFogOfWar(currentMap.second);
}

float MapOverlay::getScale(const sf::Vector2f& mapSize) const {
	return (mapSize.x / MAX_WIDTH > mapSize.y / MAX_HEIGHT) ?
		MAX_WIDTH / mapSize.x :
		MAX_HEIGHT / mapSize.y;
}

MapOverlayData* MapOverlay::createMapOverlayData(const std::string& id, const sf::Vector2i& size, const sf::Sprite& sprite) const {
	MapOverlayData* data = new MapOverlayData();
	data->mapId = id;
	data->mapSize = size;
	data->map = sprite;

	sf::Vector2f mapSize = sf::Vector2f(data->mapSize.x * TILE_SIZE_F, data->mapSize.y * TILE_SIZE_F);
	sf::Vector2f spriteSize = sf::Vector2f(static_cast<float>(data->map.getTextureRect().width), static_cast<float>(data->map.getTextureRect().height));

	// check out the limiting factor for our scale
	if (data->mapSize.x / MAX_WIDTH > data->mapSize.y / MAX_HEIGHT) {
		data->windowSize.width = MAX_WIDTH;
		data->scale = data->windowSize.width / mapSize.x;
		data->windowSize.height = data->scale * mapSize.y;
		data->windowSize.left = LEFT;
		data->windowSize.top = (WINDOW_HEIGHT - data->windowSize.height) / 2.f;
	}
	else {
		data->windowSize.height = MAX_HEIGHT;
		data->scale = data->windowSize.height / mapSize.y;
		data->windowSize.width = data->scale * mapSize.x;
		data->windowSize.top = TOP;
		data->windowSize.left = (WINDOW_WIDTH - data->windowSize.width) / 2.f;
	}

	data->map.setScale(data->windowSize.width / spriteSize.x, data->windowSize.height / spriteSize.y);

	return data;
}

void MapOverlay::renderLevelOverlay(float scale) {
	auto lScreen = dynamic_cast<LevelScreen*>(m_screen);
	auto lData = lScreen->getWorldData();

	// background (white)
	sf::Image img;
	img.create(
		static_cast<unsigned int>(std::round(lData->mapSize.x * TILE_SIZE_F * scale)),
		static_cast<unsigned int>(std::round(lData->mapSize.y * TILE_SIZE_F * scale)), COLOR_BLACK);

	float pixelSize = TILE_SIZE_F * scale;

	// draw collidable layer on top of it (simple other imgs)
	for (int i = 0; i < lData->mapSize.x; ++i) {
		for (int j = 0; j < lData->mapSize.y; ++j) {
			if (lData->collidableTilePositions[j][i]) continue;
			sf::Image img2;
			unsigned int sizeX = static_cast<unsigned int>(std::round((i + 1) * pixelSize)) - static_cast<unsigned int>(std::round(i * pixelSize));
			unsigned int sizeY = static_cast<unsigned int>(std::round((j + 1) * pixelSize)) - static_cast<unsigned int>(std::round(j * pixelSize));
			img2.create(sizeX, sizeY, COLOR_TWILIGHT_INACTIVE);
			img.copy(img2, static_cast<unsigned int>(std::round(i * pixelSize)), static_cast<unsigned int>(std::round(j * pixelSize)));
		}
	}

	// draw the important tiles etc
	/////////////////////////////////

	// dynamic tiles
	for (auto go : *lScreen->getObjects(_DynamicTile)) {
		if (auto dTile = dynamic_cast<LevelDynamicTile*>(go)) {
			if (dTile->getDynamicTileID() == LevelDynamicTileID::Modifier && dTile->getGameObjectState() == GameObjectState::Active) {
				drawOverlayTexture(img, dTile->getCenter() * scale, 0, 0);
			}
			else if (dTile->getDynamicTileID() == LevelDynamicTileID::Door && dTile->isCollidable()) {
				drawOverlayTexture(img, dTile->getCenter() * scale, 3, 0);
			}
			else if (dTile->getDynamicTileID() == LevelDynamicTileID::Chest) {
				auto chest = dynamic_cast<ChestLevelTile*>(dTile);
				if (chest && chest->isLootable()) {
					drawOverlayTexture(img, dTile->getCenter() * scale, 1, 1);
					if (chest->isQuestRelevant()) {
						drawOverlayTexture(img, dTile->getCenter() * scale, 2, 0);
					}
				}
			}
			else if (dTile->getDynamicTileID() == LevelDynamicTileID::Lever) {
				drawOverlayTexture(img, dTile->getCenter() * scale, 3, 1);
			}
			else if (dTile->getDynamicTileID() == LevelDynamicTileID::Checkpoint && dTile->getGameObjectState() == GameObjectState::Active) {
				drawOverlayTexture(img, dTile->getCenter() * scale, 4, 0);
			}
		}
	}

	// triggers
	for (auto go : *lScreen->getObjects(_Overlay)) {
		if (Trigger* trigger = dynamic_cast<Trigger*>(go)) {
			if (trigger->getData().isKeyGuarded) {
				drawOverlayTexture(img, trigger->getCenter() * scale, 1, 0);
			}
		}
	}

	// items
	for (auto go : *lScreen->getObjects(_LevelItem)) {
		if (LevelItem* item = dynamic_cast<LevelItem*>(go)) {
			auto type = item->getItemType();
			if (type == ItemType::Quest || type == ItemType::Key || type == ItemType::Spell || lScreen->isItemMonitored(item->getID())) {
				drawOverlayTexture(img, item->getCenter() * scale, 2, 0);
			}
			else {
				drawOverlayTexture(img, item->getCenter() * scale, 2, 1);
			}
		}
	}

	// enemies
	for (auto go : *lScreen->getObjects(_Enemy)) {
		if (Enemy* enemy = dynamic_cast<Enemy*>(go)) {
			if (enemy->isQuestRelevant()) {
				drawOverlayTexture(img, enemy->getCenter() * scale, 2, 0);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	// now convert to texture format
	m_levelOverlayTexture.loadFromImage(img);

	// and save as sprite
	m_levelOverlaySprite.setTexture(m_levelOverlayTexture, true);
}

void MapOverlay::drawOverlayTexture(sf::Image& image, const sf::Vector2f& pos, int posX, int posY) {
	image.copy(m_levelOverlayIcons, static_cast<unsigned int>(std::round(pos.x - 12.5)), static_cast<unsigned int>(std::round(pos.y - 12.5)),
		sf::IntRect(posX * 25, posY * 25, 25, 25), true);
}

void MapOverlay::reloadButtonGroup() {
	if (m_buttonGroup) {
		delete m_buttonGroup;
	}

	m_buttonGroup = new JoystickButtonGroup();
	m_buttonGroup->setSelectableWindow(this);
	m_buttonGroup->setGamepadEnabled(isWindowSelected());

	for (auto waypoint : m_waypoints) {
		m_buttonGroup->addButton(waypoint);
	}

	for (auto marker : m_questMarkers) {
		m_buttonGroup->addButton(marker);
	}
}

void MapOverlay::notifyLevelOverlayReload() {
	m_needsLevelOverlayReload = true;
}

void MapOverlay::reloadLevelOverlay() {
	if (!m_isLevel) return;
	if (m_maps.empty()) {
		return;
	}

	renderLevelOverlay(m_maps[0]->scale);
	m_maps[0]->map = m_levelOverlaySprite;
	m_maps[0]->map.setPosition(sf::Vector2f(m_boundingBox.left, m_boundingBox.top));
	m_needsLevelOverlayReload = false;
}

void MapOverlay::updateWindowSelected() {
	m_buttonGroup->setGamepadEnabled(isWindowSelected());

	if (isWindowSelected()) {
		m_buttonGroup->notifyJoystickDown();
	}
}

void MapOverlay::reloadMaps() {
	CLEAR_VECTOR(m_maps);

	int buttonIndex = 0;

	// load level button
	auto const lScreen = dynamic_cast<LevelScreen*>(m_screen);
	m_isLevel = lScreen != nullptr && !lScreen->getWorldData()->isBossLevel;

	if (m_isLevel) {
		sf::Vector2f mapSize = sf::Vector2f(
			lScreen->getWorldData()->mapSize.x * TILE_SIZE_F,
			lScreen->getWorldData()->mapSize.y * TILE_SIZE_F);
		renderLevelOverlay(getScale(mapSize));
		MapOverlayData* data = createMapOverlayData(lScreen->getWorldData()->id, lScreen->getWorldData()->mapSize,
			m_levelOverlaySprite);

		m_maps.push_back(data);

		sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_GUI_LEVEL_ICON);
		m_mapTabBar->setButtonTexture(buttonIndex, tex, 0);
		m_mapTabBar->setButtonOnClick(buttonIndex, std::bind(&MapOverlay::setMap, this, data->mapId));
		buttonIndex++;
		m_needsLevelOverlayReload = true;
	}

	// load map buttons
	for (auto& explored : m_screen->getCharacterCore()->getData().tilesExplored) {

		const std::string mapFilename = getMapSpriteFilename(explored.first);
		const std::string iconFilename = getMapIconFilename(explored.first);

		g_resourceManager->loadTexture(mapFilename, ResourceType::Global);
		g_resourceManager->loadTexture(iconFilename, ResourceType::Global);

		if (g_resourceManager->getTexture(mapFilename) == nullptr) continue;
		if (g_resourceManager->getTexture(iconFilename) == nullptr) continue;

		MapOverlayData* data = createMapOverlayData(explored.first, explored.second.first,
			sf::Sprite(*g_resourceManager->getTexture(mapFilename)));

		data->fogOfWarTileMap.initFogOfWar(data->mapSize);
		data->fogOfWarTileMap.setScale(data->scale, data->scale);

		// load buttons
		sf::Texture* tex = g_resourceManager->getTexture(iconFilename);
		m_mapTabBar->setButtonTexture(buttonIndex, tex, 0);
		m_mapTabBar->setButtonOnClick(buttonIndex, std::bind(&MapOverlay::setMap, this, data->mapId));

		m_maps.push_back(data);

		buttonIndex++;
	}
}

MapOverlayData* MapOverlay::getCurrentMap() const {
	if (m_currentMap < 0 || m_currentMap > static_cast<int>(m_maps.size())) return nullptr;
	return m_maps[m_currentMap];
}

WorldScreen* MapOverlay::getScreen() const {
	return m_screen;
}

void MapOverlay::reloadMarkers() {
	reloadWaypoints();
	reloadQuestMarkers();
	reloadButtonGroup();
}

void MapOverlay::reloadWaypoints() {
	for (auto& wp : m_waypoints) {
		delete wp;
	}
	m_waypoints.clear();
	if (!m_screen->getCharacterCore()->hasItem(WaypointTile::TOOL_ITEM_ID, 1)) return;
	if (m_screen->getWorldData()->isTeleportLocked) return;

	const LevelData* lData = dynamic_cast<const LevelData*>(m_screen->getWorldData());
	if (lData && lData->isBossLevel) {
		return;
	}

	auto* map = getCurrentMap();
	if (map == nullptr) return;

	if (!contains(m_screen->getCharacterCore()->getData().waypointsUnlocked, map->mapId)) return;

	for (auto& it : m_screen->getCharacterCore()->getData().waypointsUnlocked.at(map->mapId)) {
		WaypointMarker* marker = new WaypointMarker(m_screen->getMainCharacter(), it.second, this);
		marker->loadAnimation();
		marker->setPosition(m_position + (it.second + sf::Vector2f(TILE_SIZE_F * 0.19f, TILE_SIZE_F * 0.23f)) * map->scale -
			sf::Vector2f(12.5f, 12.5f));
		m_waypoints.push_back(marker);
	}
}

void MapOverlay::reloadQuestMarkers() {
	CLEAR_VECTOR(m_questMarkers);

	auto core = m_screen->getCharacterCore();
	auto map = getCurrentMap();

	for (auto questId : core->getData().questsTracked) {
		auto questData = core->getQuestData(questId);

		if (!questData) continue;
		if (core->getQuestState(questData->id) != QuestState::Started) continue;

		auto questMarkerDatas = QuestMarker::getCurrentStepData(*questData, core);
		for (auto& markerData : questMarkerDatas) {

			sf::Vector2f markerPos = markerData.position;
			if (markerData.mapId.empty()) {
				// the marker position is the position of the main character
				markerPos = m_screen->getMainCharacter()->getPosition();
			}
			else if (markerData.mapId != map->mapId) {
				// this marker is not on this map.
				continue;
			}
			else if (markerData.mapId == m_screen->getWorld()->getID() && !markerData.npcId.empty()) {
				// this marker is on the current map and references an npc on this map.
				for (auto go : *m_screen->getObjects(_MapMovableGameObject)) {
					auto const npc = dynamic_cast<NPC*>(go);
					if (npc && npc->getNPCData().id == markerData.npcId)
					{
						markerPos = npc->getPosition();
						break;
					}
				}
			}

			MapQuestMarker* marker = new MapQuestMarker(*questData, markerData, m_interface);
			marker->setActive(true);
			marker->setPosition(m_position + (markerPos * map->scale) - sf::Vector2f(QuestMarker::SIZE * 0.5f, QuestMarker::SIZE * 0.5f));
			m_questMarkers.push_back(marker);
		}
	}
}

void MapOverlay::notifyJumpToQuest(const std::string& questId, const std::vector<QuestMarkerData>& data) {
	if (data.empty()) return;

	if (data[0].mapId.empty()) {
		// special case, this means that the main character is marked on the map.
		// we set the current map now.
		setMap(m_screen->getWorldData()->id);
	}
	else {
		// set the map that's referened in the quest marker.
		if (!setMap(data[0].mapId)) return;
	}

	m_mapTabBar->show(m_currentMap);

	for (int i = 0; i < static_cast<int>(m_buttonGroup->getButtons().size()); ++i) {
		auto qm = dynamic_cast<MapQuestMarker*>(m_buttonGroup->getButtons()[i]);
		if (qm && qm->getQuestId() == questId) {
			m_buttonGroup->selectButton(i);
		}
	}
}

bool MapOverlay::isVisible() const {
	return m_isVisible;
}

bool MapOverlay::isOnCurrentMap() const {
	return m_isOnCurrentMap;
}

void MapOverlay::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	auto map = getCurrentMap();
	if (map == nullptr) return;

	target.draw(map->map);
	target.draw(map->fogOfWarTileMap);

	target.draw(m_title);

	m_window->render(target);

	for (auto wp : m_waypoints) {
		wp->render(target);
	}

	if (m_isOnCurrentMap)
		target.draw(m_mainCharMarker);

	for (auto qm : m_questMarkers) {
		qm->render(target);
	}

	for (auto qm : m_questMarkers) {
		qm->renderAfterForeground(target);
	}
}

void MapOverlay::show() {
	reloadMarkers();
	m_mapTabBar->show(m_currentMap);
	m_isVisible = true;
}

void MapOverlay::hide() {
	m_mapTabBar->hide();
	m_isVisible = false;
}

std::string MapOverlay::getMapSpriteFilename(const std::string& mapID) {
	std::string filename = mapID;
	if (filename.size() < 4) return "";
	return filename.substr(0, filename.size() - 4) + ".png";
}

std::string MapOverlay::getMapIconFilename(const std::string& mapID) {
	std::string filename = mapID;
	if (filename.size() < 4) return "";
	return filename.substr(0, filename.size() - 4) + "_icon.png";
}

/////////// WAYPOINT MARKER /////////////

WaypointMarker::WaypointMarker(MainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent) {
	m_parent = parent;
	m_mainChar = mainChar;
	m_waypointPosition = waypointPosition;
	m_isInputInDefaultView = true;
	m_screen = parent->getScreen();
}

void WaypointMarker::loadAnimation() {
	// load animations
	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 25.f));

	Animation* activeAnimation = new Animation();
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	for (int i = 2; i < 6; i++)
		activeAnimation->addFrame(sf::IntRect(i * 25, 0, 25, 25));

	for (int i = 5; i > 1; i--)
		activeAnimation->addFrame(sf::IntRect(i * 25, 0, 25, 25));

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	idleAnimation->addFrame(sf::IntRect(25, 0, 25, 25));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	m_tooltip.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_tooltip.setTextStyle(TextStyle::Shadowed);
	m_tooltip.setString(getTooltipString());
}

std::string WaypointMarker::getTooltipString() const {
	if (!g_inputController->isGamepadConnected()) {
		return g_textProvider->getText("InteractRightClick") + " " + g_textProvider->getText("ToTeleport");
	}

	auto const resolvedKey = GamepadMappings::getKeyName(Key::Confirm);
	return "<" + resolvedKey + "> " + g_textProvider->getText("ToTeleport");
}

void WaypointMarker::setPosition(const sf::Vector2f& position) {
	AnimatedGameObject::setPosition(position);
	m_tooltip.setPosition(position + sf::Vector2f(-0.5f * (m_tooltip.getBounds().width - m_boundingBox.width), -8.f));
}

void WaypointMarker::onMouseOver() {
	m_isMouseOver = true;
}

void WaypointMarker::onLeftClick() {
	click();
}

void WaypointMarker::click() {
	if (g_inputController->isActionLocked()) return;

	g_inputController->lockAction();

	if (m_parent->isOnCurrentMap()) {
		m_mainChar->setPosition(m_waypointPosition);
	}
	else {
		TriggerContent tc;
		tc.type = TriggerContentType::MapEntry;
		tc.s1 = m_parent->getCurrentMap()->mapId;
		tc.i1 = static_cast<int>(m_waypointPosition.x);
		tc.i2 = static_cast<int>(m_waypointPosition.y);

		TriggerContent::executeTrigger(tc, m_parent->getScreen());
	}

	g_resourceManager->playSound(GlobalResource::SOUND_TELEPORT);
	m_parent->hide();
}

void WaypointMarker::updateColor() {
}

void WaypointMarker::update(const sf::Time& frameTime) {
	m_isMouseOver = false;
	AnimatedGameObject::update(frameTime);
	bool activeState = m_isMouseOver || (isSelected() && m_parent->isWindowSelected());
	setState(activeState ? GameObjectState::Active : GameObjectState::Idle);
}

void WaypointMarker::render(sf::RenderTarget& target) {
	AnimatedGameObject::render(target);
	if (m_state == GameObjectState::Active) {
		target.draw(m_tooltip);
	}
}

///////////////////////////////////////////////////
//////////////// QUEST MARKER /////////////////////
///////////////////////////////////////////////////

MapQuestMarker::MapQuestMarker(const QuestData& questData, const QuestMarkerData& markerData, WorldInterface* interface) :
	QuestMarker(questData, interface->getScreen()->getCharacterCore()) {
	m_markerData = markerData;
	m_interface = interface;
	init();
}

void MapQuestMarker::update(const sf::Time& frameTime) {
	QuestMarker::update(frameTime);
	if (isSelected()) {
		setTooltipTime(sf::seconds(0.1f));
	}
}

void MapQuestMarker::onLeftClick() {
	click();
}

void MapQuestMarker::click() {
	if (g_inputController->isActionLocked()) return;
	g_inputController->lockAction();
	jumpToQuest();
}

void MapQuestMarker::onMouseOver() {
	m_isMouseOver = true;
}

void MapQuestMarker::init() {
	m_tooltipComponent->setTooltipText(g_textProvider->getText(m_questData.id, "quest"));
}

void MapQuestMarker::setTooltipTime(const sf::Time& time) {
	m_tooltipComponent->setCurrentTooltipTime(time);
}

const std::string& MapQuestMarker::getQuestId() {
	return m_questData.id;
}

void MapQuestMarker::jumpToQuest() {
	m_interface->jumpToQuestLog(m_questData.id);
}

void MapQuestMarker::updateColor() {
}

