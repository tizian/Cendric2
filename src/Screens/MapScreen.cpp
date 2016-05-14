#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"
#include "Map/NPC.h"
#include "Screens/ScreenManager.h"
#include "ScreenOverlays/ScreenOverlay.h"
#include "WeatherSystem.h"

using namespace std;

MapScreen::MapScreen(const std::string& mapID, CharacterCore* core) : WorldScreen(core) {
	m_mapID = mapID;
}

void MapScreen::execUpdate(const sf::Time& frameTime) {
	m_weatherSystem->update(m_mainChar->getPosition(), frameTime);
	if (m_currentMap.getWorldData()->explorable) {
		updateFogOfWar();
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
	updateObjects(GameObjectType::_MapMovableGameObject, frameTime);
	depthSortObjects(GameObjectType::_MapMovableGameObject, true);
	updateObjects(GameObjectType::_DynamicTile, frameTime);
	updateObjects(GameObjectType::_ForegroundDynamicTile, frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
	updateObjects(GameObjectType::_Overlay, frameTime);
	updateTooltipText(frameTime);
}

void MapScreen::loadForRenderTexture() {
	m_currentMap.loadForRenderTexture();
}

void MapScreen::load() {
	if (!(m_currentMap.load(m_mapID, this))) {
		string errormsg = m_mapID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}

	m_characterCore->initializeMapMaps(m_mapID);

	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(m_mainChar);
	m_currentMap.loadAfterMainChar();

	m_interface = new MapInterface(this);
	m_progressLog = new ProgressLog(getCharacterCore());

	g_resourceManager->playMusic(m_currentMap.getMusicPath());

	// adjust weather
	m_weatherSystem = new WeatherSystem();
	m_weatherSystem->load(m_characterCore->getWeather(m_mapID));
}

void MapScreen::execOnEnter(const Screen* previousScreen) {
	addObject(ScreenOverlay::createLocationScreenOverlay(m_currentMap.getName()));

	if (m_currentMap.getWorldData()->explorable) {
		std::map<std::string, std::vector<bool>>& tilesExplored = m_characterCore->getExploredTiles();
		if (tilesExplored.find(m_mapID) == tilesExplored.end()) {
			sf::Vector2i size = m_currentMap.getWorldData()->mapSize;
			tilesExplored[m_mapID] = std::vector<bool>(size.x * size.y, false);
		}
	}
}

void MapScreen::notifyConditionAdded(const std::string& conditionType, const std::string& condition) {
	WorldScreen::notifyConditionAdded(conditionType, condition);
	for (auto& it : *getObjects(GameObjectType::_MapMovableGameObject)) {
		if (NPC* npc = dynamic_cast<NPC*>(it)) {
			npc->reloadRoutine();
		}
	}
}

void MapScreen::execOnExit(const Screen* nextScreen) {
	WorldScreen::execOnExit(nextScreen);
	g_resourceManager->stopMusic();
	m_currentMap.dispose();
	clearOverlays();
}

const Map* MapScreen::getWorld() const {
	return &m_currentMap;
}

MapMainCharacter* MapScreen::getMainCharacter() const {
	return m_mainChar;
}

bool MapScreen::exitWorld() {
	m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
	return true;
}

void MapScreen::notifyBackFromMenu() {
	g_resourceManager->playMusic(m_currentMap.getMusicPath());
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

bool MapScreen::isOverlayActive() {
	return (m_cookingWindow != nullptr ||
		m_dialogueWindow != nullptr ||
		m_bookWindow != nullptr);
}

bool MapScreen::isOverlayVisible() {
	return isOverlayActive() ||
		m_interface->isGuiOverlayVisible();
}

void MapScreen::setDialogue(NPC* npc) {
	clearOverlays();

	m_dialogueWindow = new DialogueWindow();
	m_dialogueWindow->load(npc, this);
}

void MapScreen::setBook(const BookData* bookData) {
	clearOverlays();
	m_interface->hideAll();

	m_bookWindow = new BookWindow(*bookData, this);
	m_bookWindowDisposed = false;
	m_bookWindow->addCloseButton([&]() {
		m_bookWindowDisposed = true;
	});
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
	sf::View adjustedView = renderTarget.getView();

	// Render ambient light level + light sprites to extra buffer	(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(adjustedView);
	m_renderTexture2.setView(adjustedView);
	renderObjects(GameObjectType::_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setUniform("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Clear extra buffer
	m_renderTexture2.clear(sf::Color(0, 0, 0, 0));

	// Render foreground layer to extra buffer
	m_currentMap.drawForeground(m_renderTexture2, sf::RenderStates::Default);
	renderObjects(GameObjectType::_ForegroundDynamicTile, m_renderTexture2);
	m_renderTexture2.display();

	// Render buffer to window										(Normal foreground rendered on top)
	m_sprite.setTexture(m_renderTexture2.getTexture());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite);

	// Render extra buffer with foreground shader to window			(Ambient light level added on top of foreground)
	m_sprite.setTexture(m_renderTexture2.getTexture());
	m_foregroundLayerShader.setUniform("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_foregroundLayerShader);

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
	if (m_bookWindow != nullptr) {
		m_bookWindow->render(renderTarget);
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

void MapScreen::handleBookWindow(const sf::Time& frameTime) {
	if (m_bookWindow == nullptr) return;
	if (!m_bookWindow->updateWindow(frameTime) || m_bookWindowDisposed) {
		delete m_bookWindow;
		m_bookWindow = nullptr;
	}
	updateProgressLog(frameTime);
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
}

void MapScreen::updateFogOfWar() {
	std::vector<bool>& tilesExplored = m_characterCore->getExploredTiles().at(m_mapID);

	int range = 6;

	sf::Vector2i size = m_currentMap.getWorldData()->mapSize;
	
	sf::Vector2f pos = m_mainChar->getPosition();
	int x = static_cast<int>(pos.x / TILE_SIZE_F);
	int y = static_cast<int>(pos.y / TILE_SIZE_F);

	for (int i = x - range; i <= x + range; ++i) {
		for (int j = y - range; j < y + range; ++j) {
			if (i < 0 || i >= size.x || j < 0 || j >= size.y) continue;

			if ((x - i) * (x - i) + (y - j) * (y - j) < range * range) {
				tilesExplored[i + j * size.x] = true;
			}
		}
	}
}