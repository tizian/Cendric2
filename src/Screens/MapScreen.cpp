#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

MapScreen::MapScreen(const std::string& mapID, CharacterCore* core) : GameScreen(core) {
	m_mapID = mapID;
}

Screen* MapScreen::update(const sf::Time& frameTime) {
	// handle case where a dialogue is open
	if (m_dialogueWindow != nullptr) {
		if (!m_dialogueWindow->updateDialogue(frameTime)) {
			delete m_dialogueWindow;
			m_dialogueWindow = nullptr;
		}
		updateProgressLog(frameTime);
		updateTooltipText(frameTime);
		updateObjects(GameObjectType::_Light, frameTime);
		return this;
	}
	// handle case where a cooking window is open
	if (m_cookingWindow != nullptr) {
		if (!m_cookingWindow->updateWindow(frameTime)) {
			delete m_cookingWindow;
			m_cookingWindow = nullptr;
		}
		updateProgressLog(frameTime);
		updateTooltipText(frameTime);
		updateObjects(GameObjectType::_Light, frameTime);
		return this;
	}
	else {
		GameScreen::update(frameTime);
		if (g_inputController->isKeyJustPressed(Key::Escape)) {
			// store pos & go back to menu screen
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			return new MenuScreen(m_characterCore);
		}
		if (g_inputController->isKeyJustPressed(Key::Quickload)) {
			// store pos & go back to menu screen
			CharacterCore* newCharacterCore = new CharacterCore();
			if (!newCharacterCore->quickload()) {
				// no quicksave exists
				setTooltipText(g_textProvider->getText("NoQuicksaveExists"), sf::Color::Red, true);
				delete newCharacterCore;
			}
			else {
				delete m_characterCore;
				m_characterCore = newCharacterCore;
				return new LoadingScreen(m_characterCore);
			}
		}
		if (g_inputController->isKeyJustPressed(Key::Quicksave)) {
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->quicksave();
			setTooltipText(g_textProvider->getText("GameSaved"), sf::Color::Green, true);
		}
		MapExitData* data = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
		if (data == nullptr || m_isOnLevelEntry) {
			m_isOnLevelEntry = (data != nullptr);
			updateObjects(GameObjectType::_MainCharacter, frameTime);
			updateObjects(GameObjectType::_NPC, frameTime);
			updateObjects(GameObjectType::_DynamicTile, frameTime);
			updateObjects(GameObjectType::_Light, frameTime);
			m_currentMap.update(frameTime);
			updateTooltipText(frameTime);
			deleteDisposedObjects();
			return this;
		}
		else {
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->setLevel(data->levelSpawnPoint, data->levelID);
			delete data;
			return new LoadingScreen(getCharacterCore());
		}
	}
}

void MapScreen::loadForRenderTexture() {
	m_currentMap.loadForRenderTexture(this);
}

void MapScreen::load() {
	if (!(m_currentMap.load(m_mapID))) {
		string errormsg = m_mapID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}
	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(m_mainChar);
	m_currentMap.loadAfterMainChar(this);

	m_interface = new MapInterface(this);
	m_progressLog = new ProgressLog(getCharacterCore());
}

void MapScreen::execOnEnter(const Screen *previousScreen) {
	// nop
}

void MapScreen::execOnExit(const Screen *nextScreen) {
	m_currentMap.dispose();
	delete m_dialogueWindow;
	delete m_cookingWindow;
}

void MapScreen::setDialogue(const NPCData& data) {
	delete m_dialogueWindow;
	delete m_cookingWindow;
	m_cookingWindow = nullptr;

	m_dialogueWindow = new DialogueWindow();
	m_dialogueWindow->load(data, this);
}

void MapScreen::setCooking() {
	delete m_dialogueWindow;
	delete m_cookingWindow;
	m_dialogueWindow = nullptr;

	m_cookingWindow = new CookingWindow(this);
}

void MapScreen::render(sf::RenderTarget &renderTarget) {
	sf::Vector2f focus = m_mainChar->getCenter();

	// Render map background etc. to window							(Normal map background rendered)
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default, focus);
	renderObjects(GameObjectType::_DynamicTile, renderTarget);
	renderObjects(GameObjectType::_NPC, renderTarget);
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	m_currentMap.drawLightedForeground(renderTarget, sf::RenderStates::Default, focus);
	sf::View adjustedView = renderTarget.getView();

	// Render ambient light level + light sprites to extra buffer	(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(adjustedView);
	renderObjects(GameObjectType::_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setUniform("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Clear extra buffer
	m_renderTexture.clear(sf::Color(0, 0, 0, 0));

	// Render foreground layer to extra buffer
	m_currentMap.drawForeground(m_renderTexture, sf::RenderStates::Default, focus);
	m_renderTexture.display();

	// Render buffer to window										(Normal foreground rendered on top)
	m_sprite.setTexture(m_renderTexture.getTexture());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite);

	// Render extra buffer with foreground shader to window			(Ambient light level added on top of foreground)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_foregroundLayerShader.setUniform("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_foregroundLayerShader);

	// Render overlays on top of level; no light levels here		(GUI stuff on top of everything)
	renderTarget.setView(adjustedView);
	renderObjectsAfterForeground(GameObjectType::_DynamicTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_NPC, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_MainCharacter, renderTarget);
	renderTooltipText(renderTarget);
	GameScreen::render(renderTarget); // this will set the view to the default view!

	if (m_dialogueWindow != nullptr) {
		m_dialogueWindow->render(renderTarget);
	}
	if (m_cookingWindow != nullptr) {
		m_cookingWindow->render(renderTarget);
	}

	renderTarget.setView(adjustedView);
}