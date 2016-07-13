#include "Screens/LevelScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/ScreenManager.h"

using namespace std;

LevelScreen::LevelScreen(const string& levelID, CharacterCore* core) : WorldScreen(core) {
	m_levelID = levelID;
	m_particleRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void LevelScreen::loadForRenderTexture() {
	m_currentLevel.loadForRenderTexture();
}

void LevelScreen::load() {
	if (!(m_currentLevel.load(m_levelID, this))) {
		string errormsg = m_levelID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}

	m_characterCore->initializeLevelMaps(m_levelID);

	LevelMainCharacterLoader loader;
	m_mainChar = loader.loadMainCharacter(this, &m_currentLevel);
	m_currentLevel.loadAfterMainChar();
	loader.loadEquipment(this);
	m_progressLog = new ProgressLog(getCharacterCore());
	m_progressLog->setYOffset(150.f);
	m_interface = new LevelInterface(this, m_mainChar);
	dynamic_cast<LevelInterface*>(m_interface)->setSpellManager(m_mainChar->getSpellManager());
	dynamic_cast<LevelInterface*>(m_interface)->setPermanentCore(m_characterCore);

	// handle boss level
	const BossLevelData& data = m_currentLevel.getWorldData()->bossLevelData;
	if (data.isBossLevel) {
		if (data.isOnLoseLevel) {
			m_characterCore->setLevel(data.onLosePosition, data.onLoseWorld);
		}
		else {
			m_characterCore->setMap(data.onLosePosition, data.onLoseWorld);
		}
	}

	m_resumeButton = new Button(sf::FloatRect(450, 350, 400, 50), GUIOrnamentStyle::MEDIUM);
	m_resumeButton->setText("Resume");
	m_resumeButton->setVisible(false);
	m_resumeButton->setOnClick(std::bind(&LevelScreen::onResume, this));
	addObject(m_resumeButton);

	m_retryButton = new Button(sf::FloatRect(450, 410, 400, 50), GUIOrnamentStyle::MEDIUM);
	m_retryButton->setText("BackToCheckpoint");
	m_retryButton->setVisible(false);
	m_retryButton->setOnClick(std::bind(&LevelScreen::onBackToCheckpoint, this));
	addObject(m_retryButton);

	m_backToMenuButton = new Button(sf::FloatRect(450, 470, 400, 50), GUIOrnamentStyle::MEDIUM);
	m_backToMenuButton->setText("BackToMenu");
	m_backToMenuButton->setVisible(false);
	m_backToMenuButton->setOnClick(std::bind(&LevelScreen::onBackToMenu, this));
	addObject(m_backToMenuButton);

	g_resourceManager->playMusic(m_currentLevel.getMusicPath());

	// adjust weather
	loadWeather();
}

void LevelScreen::cleanUp() {
	g_resourceManager->stopMusic();
	m_currentLevel.dispose();
}

bool LevelScreen::exitWorld() {
	if (m_isGameOver) return false;

	return true;
}

void LevelScreen::notifyBackFromMenu() {
	m_mainChar->setQuickcast(g_resourceManager->getConfiguration().isQuickcast);
	g_resourceManager->playMusic(m_currentLevel.getMusicPath());
}

void LevelScreen::notifyBossKilled(std::map<std::string, int>& items, int gold) {
	addScreenOverlay(ScreenOverlay::createEnemyDefeatedScreenOverlay(items, gold));
	m_interface->hideAll();
	m_isBossDefeated = true;
}

void LevelScreen::clearConsumedFood() {
	dynamic_cast<LevelInterface*>(m_interface)->clearConsumedFood();
}

void LevelScreen::quicksave() {
	if (m_isGameOver) return;
	WorldScreen::quicksave();
}

void LevelScreen::execOnEnter(const Screen* previousScreen) {
	addObject(ScreenOverlay::createLocationScreenOverlay(m_currentLevel.getName(), m_currentLevel.getWorldData()->bossLevelData.isBossLevel));
}

void LevelScreen::execOnExit(const Screen* nextScreen) {
	WorldScreen::execOnExit(nextScreen);
	cleanUp();
}

void LevelScreen::addSpellBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addSpellBuff(textureLocation, duration, spell, attr);
}

void LevelScreen::addFoodBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addFoodBuff(textureLocation, duration, itemID, attr);
}

void LevelScreen::addDotBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addDotBuff(textureLocation, duration, data);
}

void LevelScreen::removeTypedBuffs(SpellID id) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().removeTypedSpellBuffs(id);
}

void LevelScreen::notifyQuickSlotAssignment(const std::string& itemId, int quickslotNr) {
	m_characterCore->setQuickslot(itemId, quickslotNr);
}

LevelMainCharacter* LevelScreen::getMainCharacter() const {
	return m_mainChar;
}

const Level* LevelScreen::getWorld() const {
	return &m_currentLevel;
}

const LevelData* LevelScreen::getWorldData() const {
	return m_currentLevel.getWorldData();
}

sf::RenderTexture& LevelScreen::getParticleRenderTexture() {
	return m_particleRenderTexture;
}

void LevelScreen::execUpdate(const sf::Time& frameTime) {
	m_weatherSystem->update(m_mainChar->getPosition(), frameTime);
	handleGameOver(frameTime);
	handleBossDefeated(frameTime);

	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);

	if (!m_isPaused) {
		WorldScreen::execUpdate(frameTime);

		// sort Movable Tiles
		depthSortObjects(GameObjectType::_MovableTile, false);
		// update objects first for relative velocity
		updateObjectsFirst(GameObjectType::_MovableTile, frameTime);
		updateObjectsFirst(GameObjectType::_LevelMainCharacter, frameTime);
		updateObjectsFirst(GameObjectType::_Enemy, frameTime);
		updateObjectsFirst(GameObjectType::_DynamicTile, frameTime);
		updateObjectsFirst(GameObjectType::_Spell, frameTime);
		// and then normally
		if (!m_interface->isGuiOverlayVisible()) {
			updateObjects(GameObjectType::_ScreenOverlay, frameTime);
		}
		updateObjects(GameObjectType::_MovableTile, frameTime);
		updateObjects(GameObjectType::_DynamicTile, frameTime);
		updateObjects(GameObjectType::_Enemy, frameTime);
		updateObjects(GameObjectType::_LevelMainCharacter, frameTime);
		updateObjects(GameObjectType::_Equipment, frameTime);
		updateObjects(GameObjectType::_Spell, frameTime);
		updateObjects(GameObjectType::_Overlay, frameTime);
		if (!m_isGameOver) updateObjects(GameObjectType::_LevelItem, frameTime);

		updateObjects(GameObjectType::_Light, frameTime);
		m_currentLevel.update(frameTime);
	}

	if (m_gamePausedOverlay) {
		m_gamePausedOverlay->update(frameTime);
	}

	// handle pause
	if (!m_isGameOver && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_isPaused = !m_isPaused;

		if (m_isPaused) {
			m_gamePausedOverlay = ScreenOverlay::createGamePausedScreenOverlay();
		}
		else {
			m_gamePausedOverlay->setPermanent(false);
		}

		m_retryButton->setVisible(m_isPaused);
		m_backToMenuButton->setVisible(m_isPaused);
		m_resumeButton->setVisible(m_isPaused);
	}
}

void LevelScreen::render(sf::RenderTarget& renderTarget) {
	m_particleRenderTexture.clear(sf::Color(0, 0, 0, 0));

	sf::Vector2f focus = m_mainChar->getCenter();

	// Render level background and content to window				(Normal level background rendered)
	m_currentLevel.drawBackgroundLayers(renderTarget, sf::RenderStates::Default, focus);
	m_currentLevel.setWorldView(renderTarget, focus);
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default);
	sf::View oldView = renderTarget.getView();
	renderObjects(GameObjectType::_DynamicTile, renderTarget);
	renderObjects(GameObjectType::_MovableTile, renderTarget);
	{
		m_particleRenderTexture.display();
		m_sprite.setTexture(m_particleRenderTexture.getTexture());
		renderTarget.setView(renderTarget.getDefaultView());
		renderTarget.draw(m_sprite);
		renderTarget.setView(oldView);
	}
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_LevelMainCharacter, renderTarget);
	renderObjects(GameObjectType::_Equipment, renderTarget);
	renderObjects(GameObjectType::_Enemy, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
	m_currentLevel.drawLightedForeground(renderTarget, sf::RenderStates::Default);
	renderObjects(GameObjectType::_DynamicTile, renderTarget); // dynamic tiles get rendered twice, this one is for the fluid tiles.

	// Render light sprites to extra buffer							(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(oldView);
	m_renderTexture2.setView(oldView);
	renderObjects(GameObjectType::_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setUniform("ambientLevel", m_currentLevel.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Clear extra buffer
	m_renderTexture2.clear(sf::Color(0, 0, 0, 0));

	// Render foreground layer to extra buffer
	m_currentLevel.drawForeground(m_renderTexture2, sf::RenderStates::Default);
	m_renderTexture2.display();

	// Render buffer to window										(Normal foreground rendered on top)
	m_sprite.setTexture(m_renderTexture2.getTexture());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite);

	// Render extra buffer with foreground shader to window			(Ambient light level added on top of foreground)
	m_sprite.setTexture(m_renderTexture2.getTexture());
	m_foregroundLayerShader.setUniform("ambientLevel", m_currentLevel.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_foregroundLayerShader);

	// Render overlays on top of level; no light levels here		(GUI stuff on top of everything)
	renderTarget.setView(oldView);
	renderObjects(GameObjectType::_Overlay, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Overlay, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_MovableTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_DynamicTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelItem, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelMainCharacter, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Equipment, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Enemy, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Spell, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Light, renderTarget);

	m_weatherSystem->render(renderTarget);

	renderTarget.setView(renderTarget.getDefaultView());
	if (!m_interface->isGuiOverlayVisible()) {
		renderObjects(GameObjectType::_ScreenOverlay, renderTarget);
	}
	if (m_gamePausedOverlay) {
		m_gamePausedOverlay->render(renderTarget);
	}
	renderTooltipText(renderTarget);
	WorldScreen::render(renderTarget);
	WorldScreen::renderAfterForeground(renderTarget);

	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);

	renderTarget.setView(oldView);
}

void LevelScreen::handleBossDefeated(const sf::Time& frameTime) {
	if (!m_isBossDefeated) return;

	if (m_bossDefeatedWaitTime == sf::Time::Zero) return;
	updateTime(m_bossDefeatedWaitTime, frameTime);
	if (m_bossDefeatedWaitTime == sf::Time::Zero) {

		const BossLevelData& data = getWorldData()->bossLevelData;
		if (data.isOnWinLevel) {
			m_characterCore->setLevel(data.onWinPosition, data.onWinWorld);
		}
		else {
			m_characterCore->setMap(data.onWinPosition, data.onWinWorld);
		}

		onBackToCheckpoint();
	}
	return;
}

void LevelScreen::handleGameOver(const sf::Time& frameTime) {
	// handle game over
	if (!m_mainChar->isDead()) return;
	if (m_isGameOver) {
		if (m_respawnWaitTime == sf::Time::Zero) return;
		updateTime(m_respawnWaitTime, frameTime);
		if (m_respawnWaitTime == sf::Time::Zero) {
			dynamic_cast<LevelInterface*>(m_interface)->restoreConsumedFood();
			onBackToCheckpoint();
		}
		return;
	}

	m_isGameOver = true;
	addScreenOverlay(ScreenOverlay::createGameOverScreenOverlay());
	m_interface->hideAll();
}

// yes or no form
void LevelScreen::onNo() {
	m_yesOrNoForm->setDisposed();
	setAllButtonsEnabled(true);
}

void LevelScreen::onYesToCheckpoint() {
	setNextScreen(new LoadingScreen(m_characterCore));
	m_yesOrNoForm->setDisposed();
}

void LevelScreen::onBackToMenu() {
	if (m_isGameOver) {
		setNextScreen(new MenuScreen(m_characterCore));
	}
	else {
		setNextScreen(new MenuScreen(m_characterCore), true);
		onResume();
	}
}

void LevelScreen::onBackToCheckpoint() {
	dynamic_cast<LevelInterface*>(m_interface)->clearConsumedFood();
	setNextScreen(new LoadingScreen(m_characterCore));
}

void LevelScreen::onResume() {
	m_retryButton->setVisible(false);
	m_backToMenuButton->setVisible(false);
	m_resumeButton->setVisible(false);
	m_isPaused = false;
	m_gamePausedOverlay->setPermanent(false);
}

void LevelScreen::onRetry() {
	delete m_yesOrNoForm;
	float width = 450;
	float height = 200;
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
	m_yesOrNoForm->setMessage("QuestionGoBackToCheckpoint");
	m_yesOrNoForm->setOnNoClicked(std::bind(&LevelScreen::onNo, this));
	m_yesOrNoForm->setOnYesClicked(std::bind(&LevelScreen::onYesToCheckpoint, this));
	addObject(m_yesOrNoForm);
	setAllButtonsEnabled(false);
	m_isPaused = false;
}