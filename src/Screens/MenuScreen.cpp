#include "Screens/MenuScreen.h"
#include "Screens/CutsceneScreen.h"

using namespace std;

MenuScreen::MenuScreen(CharacterCore* core) : Screen(core) {
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

void MenuScreen::execUpdate(const sf::Time& frameTime) {
	if ((g_inputController->isKeyJustPressed(Key::Escape) && m_characterCore == nullptr)) {
		// end the game
		m_requestQuit = true;
		return;
	}
	if (g_inputController->isKeyJustPressed(Key::Escape) && m_characterCore != nullptr) {
		// resume game
		setNextScreen(new LoadingScreen(m_characterCore));
		return;
	}

	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Undefined, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
}

void MenuScreen::setAllButtonsEnabled(bool value) {
	Screen::setAllButtonsEnabled(value);
	m_saveGameButton->setEnabled(value && (m_characterCore != nullptr));
}

void MenuScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(m_versionText);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
	renderTooltipText(renderTarget);
}

void MenuScreen::execOnEnter(const Screen *previousScreen) {
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, -80.f));
	fireBasket2->setPosition(sf::Vector2f(1028.f, -80.f));
	addObject(fireBasket1);
	addObject(fireBasket2);

	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(8);
	m_versionText.setColor(sf::Color::White);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);

	float buttonHeight = 50.f;
	float buttonWidth = 300.f;
	float xOffset = (WINDOW_WIDTH - buttonWidth) / 2.f;
	float yOffset = 200.f;
	float addYOffset = 70.f;

	// add buttons
	Button* button;
	if (m_characterCore != nullptr) {
		button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
		button->setText("Resume");
		button->setOnClick(std::bind(&MenuScreen::onResume, this));
		addObject(button);
	}
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("NewGame");
	button->setOnClick(std::bind(&MenuScreen::onNewGame, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("LoadGame");
	button->setOnClick(std::bind(&MenuScreen::onLoadGame, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("SaveGame");
	button->setEnabled(m_characterCore != nullptr);
	button->setOnClick(std::bind(&MenuScreen::onSaveGame, this));
	m_saveGameButton = button;
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("Options");
	button->setOnClick(std::bind(&MenuScreen::onOptions, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("Credits");
	button->setOnClick(std::bind(&MenuScreen::onCredits, this));
	addObject(button);
	yOffset += addYOffset;

	button = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	button->setText("Exit");
	button->setOnClick(std::bind(&MenuScreen::onExit, this));
	addObject(button);
}

void MenuScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
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
	m_requestQuit = true;
}

void MenuScreen::onResume() {
	setNextScreen(new LoadingScreen(m_characterCore));
}

void MenuScreen::onNewGame() {
	if (m_yesOrNoForm != nullptr) return;
	if (m_characterCore == nullptr) {
		// we start a new game with an empty character core
		m_characterCore = new CharacterCore();
		m_characterCore->loadNew();
		Screen* nextScreen = new CutsceneScreen(m_characterCore, "intro");
		nextScreen->addScreenOverlay(ScreenOverlay::createQuestScreenOverlay("who_am_i", QuestState::Started));
		nextScreen->addScreenOverlay(ScreenOverlay::createHintScreenOverlay("Journal"));
		setNextScreen(nextScreen);;
	}
	else {
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
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