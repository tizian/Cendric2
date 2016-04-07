#include "Screens/LevelScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/ScreenManager.h"

using namespace std;

LevelScreen::LevelScreen(const string& levelID, CharacterCore* core) : WorldScreen(core) {
	m_levelID = levelID;
}

void LevelScreen::loadForRenderTexture() {
	m_currentLevel.loadForRenderTexture();
}

void LevelScreen::load() {
	delete m_characterCoreCopy;
	m_characterCoreCopy = new CharacterCore(m_characterCore->getData());

	if (!(m_currentLevel.load(m_levelID, this))) {
		string errormsg = m_levelID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}

	m_characterCoreCopy->initializeLevelMaps(m_levelID);

	LevelMainCharacterLoader loader;
	m_mainChar = loader.loadMainCharacter(this, &m_currentLevel);
	m_currentLevel.loadAfterMainChar();
	loader.loadEquipment(this);
	m_progressLog = new ProgressLog(getCharacterCore());
	m_interface = new LevelInterface(this, m_mainChar);
	dynamic_cast<LevelInterface*>(m_interface)->setSpellManager(m_mainChar->getSpellManager());

	m_resumeButton = new Button(sf::FloatRect(450, 350, 400, 50), ButtonOrnamentStyle::MEDIUM);
	m_resumeButton->setText("Resume");
	m_resumeButton->setVisible(false);
	m_resumeButton->setOnClick(std::bind(&LevelScreen::onResume, this));
	addObject(m_resumeButton);

	m_retryButton = new Button(sf::FloatRect(450, 410, 400, 50), ButtonOrnamentStyle::MEDIUM);
	m_retryButton->setText("BackToCheckpoint");
	m_retryButton->setVisible(false);
	m_retryButton->setOnClick(std::bind(&LevelScreen::onBackToCheckpoint, this));
	addObject(m_retryButton);

	m_backToMenuButton = new Button(sf::FloatRect(450, 470, 400, 50), ButtonOrnamentStyle::MEDIUM);
	m_backToMenuButton->setText("BackToMenu");
	m_backToMenuButton->setVisible(false);
	m_backToMenuButton->setOnClick(std::bind(&LevelScreen::onBackToMenu, this));
	addObject(m_backToMenuButton);

	m_overlaySprite = new sf::Sprite(*g_resourceManager->getTexture(ResourceID::Texture_screen_overlay));

	m_overlayText = new BitmapText(g_textProvider->getText("GamePaused"));
	m_overlayText->setTextStyle(TextStyle::Shadowed);
	m_overlayText->setCharacterSize(56);
	m_overlayText->setColor(COLOR_BAD);
	m_overlayText->setPosition(sf::Vector2f(std::max(0.f, (WINDOW_WIDTH - m_overlayText->getLocalBounds().width) / 2.f), 200.f));

	g_resourceManager->playMusic(m_backgroundMusic, m_currentLevel.getMusicPath());
}

void LevelScreen::cleanUp() {
	m_backgroundMusic.stop();
	m_currentLevel.dispose();
	delete m_characterCoreCopy;
	m_characterCoreCopy = nullptr;
	delete m_overlaySprite;
	delete m_overlayText;
}

CharacterCore* LevelScreen::getCharacterCore() const {
	if (m_characterCoreCopy == nullptr) {
		return m_characterCore;
	}
	return m_characterCoreCopy;
}

void LevelScreen::writeToCore() {
	delete m_characterCore;
	m_characterCore = new CharacterCore(m_characterCoreCopy->getData());
}

bool LevelScreen::exitWorld() {
	if (m_isGameOver) return false;

	writeToCore();
	delete m_characterCoreCopy;
	m_characterCoreCopy = nullptr;
	return true;
}

void LevelScreen::execOnEnter(const Screen* previousScreen) {
	addObject(ScreenOverlay::createLocationScreenOverlay(m_currentLevel.getName()));
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

LevelMainCharacter* LevelScreen::getMainCharacter() const {
	return m_mainChar;
}

const Level* LevelScreen::getWorld() const {
	return &m_currentLevel;
}

void LevelScreen::execUpdate(const sf::Time& frameTime) {
	handleGameOver(frameTime);

	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);
	if (!m_retryButton->isVisible()) {
		WorldScreen::execUpdate(frameTime);
	}

	if (!m_isGameOver && m_retryButton->isEnabled() && g_inputController->isKeyJustPressed(Key::Escape)) {
		if (m_retryButton->isVisible()) {
			m_retryButton->setVisible(false);
			m_backToMenuButton->setVisible(false);
			m_resumeButton->setVisible(false);
		}
		else {
			m_retryButton->setVisible(true);
			m_backToMenuButton->setVisible(true);
			m_resumeButton->setVisible(true);
		}
	}

	if (m_isGameOver || !m_retryButton->isVisible()) {
		// sort Movable Tiles
		depthSortObjects(GameObjectType::_MovableTile, false);
		// update objects first for relative velocity
		updateObjectsFirst(GameObjectType::_MovableTile, frameTime);
		updateObjectsFirst(GameObjectType::_LevelMainCharacter, frameTime);
		updateObjectsFirst(GameObjectType::_Enemy, frameTime);
		updateObjectsFirst(GameObjectType::_DynamicTile, frameTime);
		updateObjectsFirst(GameObjectType::_Spell, frameTime);
		// and then normally
		updateObjects(GameObjectType::_ScreenOverlay, frameTime);
		updateObjects(GameObjectType::_MovableTile, frameTime);
		updateObjects(GameObjectType::_DynamicTile, frameTime);
		updateObjects(GameObjectType::_Enemy, frameTime);
		updateObjects(GameObjectType::_LevelMainCharacter, frameTime);
		updateObjects(GameObjectType::_LevelEquipment, frameTime);
		updateObjects(GameObjectType::_Spell, frameTime);
		updateObjects(GameObjectType::_Overlay, frameTime);
		if (!m_isGameOver) updateObjects(GameObjectType::_LevelItem, frameTime);

		updateObjects(GameObjectType::_Light, frameTime);
		m_currentLevel.update(frameTime);
	}
}

void LevelScreen::render(sf::RenderTarget& renderTarget) {
	sf::Vector2f focus = m_mainChar->getCenter();

	// Render level background and content to window				(Normal level background rendered)
	m_currentLevel.drawBackgroundLayers(renderTarget, sf::RenderStates::Default, focus);
	m_currentLevel.setWorldView(renderTarget, focus);
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default);
	sf::View oldView = renderTarget.getView();
	renderObjects(GameObjectType::_MovableTile, renderTarget);
	renderObjects(GameObjectType::_DynamicTile, renderTarget);
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_LevelMainCharacter, renderTarget);
	renderObjects(GameObjectType::_LevelEquipment, renderTarget);
	renderObjects(GameObjectType::_Enemy, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
	renderObjects(GameObjectType::_DynamicTile, renderTarget); // dynamic tiles get rendered twice, this one is for the fluid tiles.
	m_currentLevel.drawLightedForeground(renderTarget, sf::RenderStates::Default);

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
	renderObjectsAfterForeground(GameObjectType::_MovableTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_DynamicTile, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelItem, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelMainCharacter, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelEquipment, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Enemy, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Spell, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Light, renderTarget);

	renderTarget.setView(renderTarget.getDefaultView());
	renderObjects(GameObjectType::_ScreenOverlay, renderTarget);
	renderTooltipText(renderTarget);
	WorldScreen::render(renderTarget);

	if (m_retryButton->isVisible()) {
		renderTarget.draw(*m_overlaySprite);
		renderTarget.draw(*m_overlayText);
	}

	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);

	renderTarget.setView(oldView);
}

void LevelScreen::handleGameOver(const sf::Time& frameTime) {
	// handle game over
	if (m_isGameOver || !m_mainChar->isDead()) return;

	m_isGameOver = true;
	m_overlaySprite->setTexture(*g_resourceManager->getTexture(ResourceID::Texture_screen_gameover));
	m_overlayText->setString(g_textProvider->getText("YouDied"));
	m_overlayText->setTextStyle(TextStyle::Shadowed);
	m_overlayText->setPosition(sf::Vector2f(std::max(0.f, (WINDOW_WIDTH - m_overlayText->getLocalBounds().width) / 2.f), 200.f));
	m_retryButton->setVisible(true);
	m_backToMenuButton->setVisible(true);
	// update once to set hp bar down
	m_interface->update(frameTime);
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
	setNextScreen(new LoadingScreen(m_characterCore));
}

void LevelScreen::onResume() {
	m_retryButton->setVisible(false);
	m_backToMenuButton->setVisible(false);
	m_resumeButton->setVisible(false);
}

void LevelScreen::onRetry() {
	if (m_isGameOver) {
		onBackToCheckpoint();
	}
	else {
		delete m_yesOrNoForm;
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
		m_yesOrNoForm->setMessage("QuestionGoBackToCheckpoint");
		m_yesOrNoForm->setOnNoClicked(std::bind(&LevelScreen::onNo, this));
		m_yesOrNoForm->setOnYesClicked(std::bind(&LevelScreen::onYesToCheckpoint, this));
		addObject(m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
}