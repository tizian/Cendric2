#include "Screens/SaveGameScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

SaveGameScreen::SaveGameScreen(CharacterCore* core) : Screen(core) {
	// precondition: character core can't be nullptr here.
	assert(core != nullptr);
}

SaveGameScreen::~SaveGameScreen() {
	delete m_saveGameWindow;
}

void SaveGameScreen::setAllButtonsEnabled(bool value) {
	Screen::setAllButtonsEnabled(value);
	bool empty = m_saveGameWindow->getChosenFilename().empty();
	m_saveButton->setEnabled(value && !empty);
	m_deleteSaveGameButton->setEnabled(value && !empty);
	m_saveGameWindow->setEnabled(value);
}

void SaveGameScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		onBack();
		return;
	}
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);
	m_saveGameWindow->update(frameTime);
	if (!getObjects(GameObjectType::_Form)->empty()) return;
	if (m_saveGameWindow->isChosen()) {
		onSaveGame();
	}
}

void SaveGameScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderTooltipText(renderTarget);
	m_saveGameWindow->render(renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
}

void SaveGameScreen::execOnEnter(const Screen *previousScreen) {
	// text
	m_title = new BitmapText(g_textProvider->getText("SaveGame"));
	m_title->setCharacterSize(24);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	// add buttons
	const float buttonWidth = 200.f;
	const float buttonHeight = 50.f;
	const float marginX = 60.f;
	const float marginY = WINDOW_HEIGHT - 80.f;
	const float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	const float buttonSpacing = (buttonSpaceWidth - 4 * buttonWidth) / 3.f;

	Button* button = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Back");
	button->setOnClick(std::bind(&SaveGameScreen::onBack, this));
	addObject(button);

	m_deleteSaveGameButton = new Button(sf::FloatRect(buttonWidth + buttonSpacing + marginX, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	m_deleteSaveGameButton->setText("Delete");
	m_deleteSaveGameButton->setOnClick(std::bind(&SaveGameScreen::onDeleteSaveGame, this));
	addObject(m_deleteSaveGameButton);

	button = new Button(sf::FloatRect(marginX + 2 * buttonWidth + 2 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("New");
	button->setOnClick(std::bind(&SaveGameScreen::onNewSaveGame, this));
	addObject(button);

	m_saveButton = new Button(sf::FloatRect(marginX + 3 * buttonWidth + 3 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	m_saveButton->setText("Save");
	m_saveButton->setOnClick(std::bind(&SaveGameScreen::onSaveGame, this));
	addObject(m_saveButton);

	// savegame window
	m_saveGameWindow = new SaveGameWindow();
	setAllButtonsEnabled(true);
}

void SaveGameScreen::execOnExit(const Screen *nextScreen) {
	delete m_title;
}

// <<< functions for agents >>>

// yes or no forms
void SaveGameScreen::onNo() {
	m_yesOrNoForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onYesOverwriteSaveGame() {
	m_yesOrNoForm = nullptr;
	if (m_characterCore->save(m_saveGameWindow->getChosenFilename(), m_saveGameWindow->getChosenSaveName())) {
		setTooltipText("GameSaved", COLOR_GOOD, true);
	}
	else {
		g_logger->logError("SaveGameScreen", "Savegame could not be saved");
		setTooltipText("OperationFailed", COLOR_BAD, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onYesDeleteSaveGame() {
	m_yesOrNoForm = nullptr;
	if (remove(m_saveGameWindow->getChosenFilename().c_str()) == 0) {
		setTooltipText("SavegameDeleted", COLOR_LIGHT_PURPLE, true);
	}
	else {
		g_logger->logError("SaveGameScreen", "Savegame could not be deleted");
		setTooltipText("OperationFailed", COLOR_BAD, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

// new save game form
void SaveGameScreen::onCancel() {
	m_newSaveGameForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onYesNewSaveGame() {
	string name = m_newSaveGameForm->getSavegameName();
	string cleanedName = name;

	string illegalChars = "\\/:?\"<>|*%.";
	for (auto& it : cleanedName) {
		bool found = illegalChars.find(it) != string::npos;
		if (found) it = ' ';
	}

	std::string file = "saves/" + to_string(time(nullptr)) + cleanedName + ".sav";

	m_newSaveGameForm = nullptr;
	if (m_characterCore->save(file, name)) {
		setTooltipText("GameSaved", COLOR_GOOD, true);
	}
	else {
		g_logger->logError("SaveGameScreen", "Savegame could not be created");
		setTooltipText("OperationFailed", COLOR_BAD, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

// buttons
void SaveGameScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}

void SaveGameScreen::onDeleteSaveGame() {
	float width = 450;
	float height = 200;
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
	m_yesOrNoForm->setMessage("QuestionDeleteSaveGame");
	m_yesOrNoForm->setOnNoClicked(std::bind(&SaveGameScreen::onNo, this));
	m_yesOrNoForm->setOnYesClicked(std::bind(&SaveGameScreen::onYesDeleteSaveGame, this));
	addObject(m_yesOrNoForm);
	setAllButtonsEnabled(false);
}

void SaveGameScreen::onNewSaveGame() {
	float width = 450;
	float height = 200;
	m_newSaveGameForm = new NewSaveGameForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
	m_newSaveGameForm->setOnOkClicked(std::bind(&SaveGameScreen::onYesNewSaveGame, this));
	m_newSaveGameForm->setOnCancelClicked(std::bind(&SaveGameScreen::onCancel, this));
	addObject(m_newSaveGameForm);
	setAllButtonsEnabled(false);
}

void SaveGameScreen::onSaveGame() {
	float width = 450;
	float height = 200;
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
	m_yesOrNoForm->setMessage("QuestionOverwriteSaveGame");
	m_yesOrNoForm->setOnNoClicked(std::bind(&SaveGameScreen::onNo, this));
	m_yesOrNoForm->setOnYesClicked(std::bind(&SaveGameScreen::onYesOverwriteSaveGame, this));
	addObject(m_yesOrNoForm);
	setAllButtonsEnabled(false);
}
