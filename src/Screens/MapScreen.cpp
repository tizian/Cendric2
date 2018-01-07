#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"
#include "Map/NPC.h"
#include "Map/MapMainCharacterLoader.h"
#include "ScreenOverlays/ScreenOverlay.h"
#include "Map/MapInterface.h"
#include "GUI/BookWindow.h"

MapScreen::MapScreen(const std::string& mapId, CharacterCore* core) : Screen(core), WorldScreen(core) {
	m_mapID = mapId;
}

void MapScreen::execUpdate(const sf::Time& frameTime) {
	m_weatherSystem->update(m_mainChar->getPosition(), frameTime);
	if (m_currentMap.getWorldData()->explorable) {
		updateFogOfWar();
	}

	if (m_dialogueWindow || m_cookingWindow) {
		m_progressLog->setYOffset(290.f);
	}
	else {
		m_progressLog->setYOffset(89.f);
	}

	handleCookingWindow(frameTime);
	handleDialogueWindow(frameTime);
	handleBookWindow(frameTime);
	m_currentMap.update(frameTime);
	if (isOverlayActive()) return;

	WorldScreen::execUpdate(frameTime);
	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		// store pos & go back to menu screen
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		setNextScreen(new MenuScreen(m_characterCore), true);
		return;
	}

	if (!m_interface->isGuiOverlayVisible()) {
		updateObjects(GameObjectType::_ScreenOverlay, frameTime);
	}
	updateObjects(GameObjectType::_DynamicTile, frameTime);
	updateObjects(GameObjectType::_ForegroundDynamicTile, frameTime);
	updateObjects(GameObjectType::_MapMovableGameObject, frameTime);
	depthSortObjects(GameObjectType::_MapMovableGameObject, true);
	updateObjects(GameObjectType::_Equipment, frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
	updateObjects(GameObjectType::_Overlay, frameTime);
	updateTooltipText(frameTime);
}

void MapScreen::loadSync() {
	m_currentMap.loadForRenderTexture();

	m_interface = new MapInterface(this);
	m_progressLog = new ProgressLog(getCharacterCore());
	updateMonitoredQuestItems();
}

void MapScreen::loadAsync() {
	if (!(m_currentMap.load(m_mapID, this))) {
		auto const errormsg = m_mapID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}

	// set overworld
	if (m_currentMap.getWorldData()->explorable) {
		m_characterCore->setOverworld(m_mapID);
	}

	m_mainChar = MapMainCharacterLoader::loadMainCharacter(this, &m_currentMap);
	m_currentMap.loadAfterMainChar(m_mainChar);
	MapMainCharacterLoader::loadEquipment(this);

	if (m_currentMap.getWorldData()->explorable) {
		ExploredTiles& tilesExplored = m_characterCore->getExploredTiles();
		bool isContained = false;
		for (auto& it : tilesExplored) {
			if (it.first == m_mapID) {
				isContained = true;
				break;
			}
		}

		if (!isContained) {
			sf::Vector2i size = m_currentMap.getWorldData()->mapSize;
			tilesExplored.push_back({ m_mapID, { size, std::vector<bool>(size.x * size.y, false)} });
		}
	}

	g_resourceManager->playMusic(m_currentMap.getMusicPath());

	// adjust weather
	loadWeather();
}

void MapScreen::execOnEnter() {
	addObject(ScreenOverlay::createLocationScreenOverlay(m_currentMap.getName()));
}

void MapScreen::notifyConditionAdded(const Condition& condition) {
	WorldScreen::notifyConditionAdded(condition);
	for (auto& it : *getObjects(GameObjectType::_MapMovableGameObject)) {
		if (NPC* npc = dynamic_cast<NPC*>(it)) {
			npc->notifyReloadNeeded();
		}
	}
}

void MapScreen::notifyItemEquip(const std::string& itemID, ItemType type) {
	WorldScreen::notifyItemEquip(itemID, type);
	MapMainCharacterLoader::loadEquipment(this);
	for (auto& it : *getObjects(GameObjectType::_MapMovableGameObject)) {
		if (NPC* npc = dynamic_cast<NPC*>(it)) {
			npc->notifyReloadNeeded();
		}
	}
}

void MapScreen::execOnExit() {
	WorldScreen::execOnExit();
	m_currentMap.dispose();
	clearOverlays();
}

const Map* MapScreen::getWorld() const {
	return &m_currentMap;
}

const MapData* MapScreen::getWorldData() const {
	return m_currentMap.getWorldData();
}

MapMainCharacter* MapScreen::getMainCharacter() const {
	return m_mainChar;
}

bool MapScreen::exitWorld() {
	m_characterCore->setMap(m_mainChar->getPreviousPosition(), m_currentMap.getID());
	return true;
}

void MapScreen::notifyBackFromMenu() {
	g_resourceManager->playMusic(m_currentMap.getMusicPath());
}

void MapScreen::notifyWaypointUnlocked() const {
	m_interface->reloadMapWaypoints();
}

void MapScreen::quicksave() {
	m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
	WorldScreen::quicksave();
}

void MapScreen::clearOverlays() {
	delete m_cookingWindow;
	delete m_dialogueWindow;
	delete m_bookWindow;
	m_cookingWindow = nullptr;
	m_dialogueWindow = nullptr;
	m_bookWindow = nullptr;
}

bool MapScreen::isOverlayActive() const {
	return (m_cookingWindow != nullptr ||
		m_dialogueWindow != nullptr ||
		m_bookWindow != nullptr);
}

bool MapScreen::isOverlayVisible() const {
	return isOverlayActive() ||
		m_interface->isGuiOverlayVisible();
}

void MapScreen::setDialogue(NPC* npc) {
	clearOverlays();
	m_interface->hideAll();

	m_dialogueWindow = new DialogueWindow();
	m_dialogueWindow->load(npc, this);
}

void MapScreen::setCooking() {
	clearOverlays();

	m_cookingWindow = new CookingWindow(this);
}

void MapScreen::render(sf::RenderTarget& renderTarget) {
	sf::Vector2f focus = m_mainChar->getCenter();

	// Render map background etc. to window							(Normal map background rendered)
	m_currentMap.setWorldView(renderTarget, focus);
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default);
	renderObjects(GameObjectType::_DynamicTile, renderTarget);
	renderObjects(GameObjectType::_MapMovableGameObject, renderTarget);
	m_currentMap.drawLightedForeground(renderTarget, sf::RenderStates::Default);
	m_currentMap.drawForeground(renderTarget, sf::RenderStates::Default);
	renderObjects(GameObjectType::_ForegroundDynamicTile, renderTarget);
	sf::View adjustedView = renderTarget.getView();

	// Render ambient light level + light sprites to extra buffer	(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(adjustedView);
	renderObjects(GameObjectType::_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setUniform("ambientLevel", m_currentMap.getWeather().ambientDimming);
	m_lightLayerShader.setUniform("lightDimming", m_currentMap.getWeather().lightDimming);
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Render overlays on top of level; no light levels here		(GUI stuff on top of everything)
	renderTarget.setView(adjustedView);
	renderObjectsAfterForeground(GameObjectType::_DynamicTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_ForegroundDynamicTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_MapMovableGameObject, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Overlay, renderTarget);

	m_weatherSystem->render(renderTarget);

	renderTooltipText(renderTarget);
	WorldScreen::render(renderTarget); // this will set the view to the default view!
	WorldScreen::renderAfterForeground(renderTarget);
	if (!isOverlayVisible()) {
		renderObjects(GameObjectType::_ScreenOverlay, renderTarget);
	}

	if (m_dialogueWindow != nullptr) {
		m_dialogueWindow->render(renderTarget);
	}
	if (m_cookingWindow != nullptr) {
		m_cookingWindow->render(renderTarget);
	}

	renderTarget.setView(adjustedView);
}

void MapScreen::handleCookingWindow(const sf::Time& frameTime) {
	if (m_cookingWindow == nullptr) return;
	if (!m_cookingWindow->updateWindow(frameTime)) {
		delete m_cookingWindow;
		m_cookingWindow = nullptr;
	}
	updateProgressLog(frameTime);
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
}

void MapScreen::handleDialogueWindow(const sf::Time& frameTime) {
	if (m_dialogueWindow == nullptr) return;
	if (!m_dialogueWindow->updateDialogue(frameTime)) {
		delete m_dialogueWindow;
		m_dialogueWindow = nullptr;
	}
	updateProgressLog(frameTime);
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
}

void MapScreen::updateFogOfWar() {
	std::pair<sf::Vector2i, std::vector<bool>>* tilesExplored = nullptr;
	for (auto& it : m_characterCore->getExploredTiles()) {
		if (it.first.compare(m_mapID) == 0) {
			tilesExplored = &it.second;
		}
	}
	if (!tilesExplored) return;

	int range = 6;

	sf::Vector2i size = m_currentMap.getWorldData()->mapSize;

	sf::Vector2f pos = m_mainChar->getPosition();
	int x = static_cast<int>(pos.x / TILE_SIZE_F);
	int y = static_cast<int>(pos.y / TILE_SIZE_F);

	for (int i = x - range; i <= x + range; ++i) {
		for (int j = y - range; j < y + range; ++j) {
			if (i < 0 || i >= size.x || j < 0 || j >= size.y) continue;

			if ((x - i) * (x - i) + (y - j) * (y - j) < range * range) {
				tilesExplored->second[i + j * size.x] = true;
			}
		}
	}
}

void MapScreen::renderEquipment(sf::RenderTarget& renderTarget) {
	renderObjects(GameObjectType::_Equipment, renderTarget);
}
