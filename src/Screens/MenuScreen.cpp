#include "Screens/MenuScreen.h"
#include "Screens/CutsceneScreen.h"
#include "Screens/ScreenManager.h"
#include "GUI/GUIConstants.h"

using namespace std;

const std::string SPRITE_PATH_BG = "res/assets/screens/screen_menu_bg.png";
const std::string SPRITE_PATH_FG = "res/assets/screens/screen_splash_fg.png";

MenuScreen::MenuScreen(CharacterCore* core) : Screen(core) {
	g_resourceManager->loadTexture(SPRITE_PATH_BG, ResourceType::Unique, this);
	m_screenSpriteBackground = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH_BG)));
	m_screenSpriteForeground = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH_FG)));
}

MenuScreen::~MenuScreen() {
	delete m_ps_left;
	delete m_ps_right;
}

void MenuScreen::setFireParticles(particles::TextureParticleSystem* ps_left, particles::TextureParticleSystem* ps_right) {
	delete m_ps_left;
	delete m_ps_right;
	m_ps_left = ps_left;
	m_ps_right = ps_right;
}

void MenuScreen::execUpdate(const sf::Time& frameTime) {
	if ((g_inputController->isKeyJustPressed(Key::Escape) && m_characterCore == nullptr)) {
		// end the game
		m_screenManager->requestQuit();
		return;
	}
	if (g_inputController->isKeyJustPressed(Key::Escape) && m_characterCore != nullptr) {
		// resume game
		m_screenManager->resumeBackupScreen();
		return;
	}

	updateTooltipText(frameTime);

	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	m_ps_left->update(frameTime);
	m_ps_right->update(frameTime);
}

void MenuScreen::setAllButtonsEnabled(bool value) {
	Screen::setAllButtonsEnabled(value);
	m_saveGameButton->setEnabled(value && (m_characterCore != nullptr));
}

void MenuScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSpriteBackground);
	m_ps_left->render(renderTarget);
	m_ps_right->render(renderTarget);
	renderTarget.draw(m_screenSpriteForeground);
	renderTarget.draw(m_versionText);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
	renderTooltipText(renderTarget);
}

void MenuScreen::execOnEnter(const Screen *previousScreen) {
	// add fire particles
	if (m_ps_left == nullptr && m_ps_right == nullptr) {
		m_ps_right = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
		m_ps_left = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
		SplashScreen::loadFireParticles(m_ps_left, sf::Vector2f(155.f, 330.f));
		SplashScreen::loadFireParticles(m_ps_right, sf::Vector2f(1130.f, 330.f));
	}
	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_versionText.setColor(COLOR_WHITE);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);

	float buttonHeight = 50.f;
	float buttonWidth = 300.f;
	float xOffset = (WINDOW_WIDTH - buttonWidth) / 2.f;
	float yOffset = 220.f;
	float addYOffset = 60.f;

	// add buttons
	Button* button;
	if (m_characterCore != nullptr) {
		button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
		button->setText("Resume");
		button->setOnClick(std::bind(&MenuScreen::onResume, this));
		addObject(button);
	}
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("NewGame");
	button->setOnClick(std::bind(&MenuScreen::onNewGame, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("LoadGame");
	button->setOnClick(std::bind(&MenuScreen::onLoadGame, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("SaveGame");
	button->setEnabled(m_characterCore != nullptr);
	button->setOnClick(std::bind(&MenuScreen::onSaveGame, this));
	m_saveGameButton = button;
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("Options");
	button->setOnClick(std::bind(&MenuScreen::onOptions, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("Credits");
	button->setOnClick(std::bind(&MenuScreen::onCredits, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), GUIOrnamentStyle::MEDIUM);
	button->setText("Exit");
	button->setOnClick(std::bind(&MenuScreen::onExit, this));
	addObject(button);
}

void MenuScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteUniqueResources(this);
}

// <<< agents for the yes or no form and other buttons >>>

void MenuScreen::onStartNewGame() {
	m_yesOrNoForm->setDisposed();
	m_yesOrNoForm = nullptr;
	delete m_characterCore;
	m_characterCore = nullptr;
	onNewGame();
}

void MenuScreen::onNo() {
	m_yesOrNoForm = nullptr;
	setAllButtonsEnabled(true);
}

void MenuScreen::onExit() {
	m_screenManager->requestQuit();
}

void MenuScreen::onResume() {
	m_screenManager->resumeBackupScreen();
}

void MenuScreen::onNewGame() {
	if (m_yesOrNoForm != nullptr) return;
	if (m_characterCore == nullptr) {
		// we start a new game with an empty character core
		m_characterCore = new CharacterCore();
		m_characterCore->loadNew();
		Screen* nextScreen = new CutsceneScreen(m_characterCore, "intro");
		setNextScreen(nextScreen);;
	}
	else {
		float width = 450;
	float height = 200;
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
		m_yesOrNoForm->setMessage("QuestionStartNewGame");
		m_yesOrNoForm->setOnNoClicked(std::bind(&MenuScreen::onNo, this));
		m_yesOrNoForm->setOnYesClicked(std::bind(&MenuScreen::onStartNewGame, this));
		addObject(m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
}

void MenuScreen::onLoadGame() {
	setNextScreen(new LoadGameScreen(m_characterCore));
}

void MenuScreen::onSaveGame() {
	if (m_characterCore == nullptr) return;
	setNextScreen(new SaveGameScreen(m_characterCore));
}

void MenuScreen::onOptions() {
	setNextScreen(new OptionsScreen(m_characterCore));
}

void MenuScreen::onCredits() {
	setNextScreen(new CreditsScreen(m_characterCore));
}