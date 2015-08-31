#include "Screens/SaveGameScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

SaveGameScreen::SaveGameScreen(CharacterCore* core) : Screen(core)
{
	// precondition: character core can't be nullptr here.
	assert(core != nullptr);
}

void SaveGameScreen::setAllButtonsEnabled(bool value)
{
	Screen::setAllButtonsEnabled(value);
	bool empty = m_saveGameWindow->getChosenFilename().empty();
	m_saveButton->setEnabled(value && !empty);
	m_deleteSaveGameButton->setEnabled(value && !empty);
	m_saveGameWindow->setEnabled(value);
}

Screen* SaveGameScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}
	updateObjects(GameObjectType::_Window, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);
	deleteDisposedObjects();
	if (!getObjects(GameObjectType::_Form)->empty()) return this;
	if (m_saveButton->isClicked() || m_saveGameWindow->isChosen())
	{
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
		m_yesOrNoForm->setMessage("QuestionOverwriteSaveGame");
		m_yesOrNoForm->setOnNoClicked(std::bind(&SaveGameScreen::onNo, this)); 
		m_yesOrNoForm->setOnYesClicked(std::bind(&SaveGameScreen::onOverwriteSaveGame, this));
		addObject(m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
	else if (m_newSaveGameButton->isClicked())
	{
		m_newSaveGameForm = new NewSaveGameForm(sf::FloatRect(400, 350, 450, 200));
		m_newSaveGameForm->setOnOkClicked(std::bind(&SaveGameScreen::onNewSaveGame, this));
		m_newSaveGameForm->setOnCancelClicked(std::bind(&SaveGameScreen::onCancel, this));
		addObject(m_newSaveGameForm);
		setAllButtonsEnabled(false);
	}
	else if (m_deleteSaveGameButton->isClicked())
	{
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
		m_yesOrNoForm->setMessage("QuestionDeleteSaveGame");
		m_yesOrNoForm->setOnNoClicked(std::bind(&SaveGameScreen::onNo, this));
		m_yesOrNoForm->setOnYesClicked(std::bind(&SaveGameScreen::onDeleteSaveGame, this));
		addObject(m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
	return this;
}

void SaveGameScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderTooltipText(renderTarget);
	renderObjects(GameObjectType::_Window, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
}

void SaveGameScreen::execOnEnter(const Screen *previousScreen)
{
	// text
	m_title = new BitmapText(g_textProvider->getText("SaveGame"));
	m_title->setCharacterSize(24);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	// add buttons
	const float buttonWidth = 200.f;
	const float buttonHeight = 50.f;
	const float marginX = 60.f;
	const float marginY = WINDOW_HEIGHT - 100.f;
	const float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	const float buttonSpacing = (buttonSpaceWidth - 4 * buttonWidth) / 3.f;

	m_backButton = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight));
	m_backButton->setText("Back");
	addObject(m_backButton);

	m_deleteSaveGameButton = new Button(sf::FloatRect(buttonWidth + buttonSpacing + marginX, marginY, buttonWidth, buttonHeight));
	m_deleteSaveGameButton->setText("Delete");
	addObject(m_deleteSaveGameButton);

	m_newSaveGameButton = new Button(sf::FloatRect(marginX + 2 * buttonWidth + 2 * buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_newSaveGameButton->setText("New");
	addObject(m_newSaveGameButton);

	m_saveButton = new Button(sf::FloatRect(marginX + 3 * buttonWidth + 3 * buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_saveButton->setText("Save");
	addObject(m_saveButton);

	// savegame window
	m_saveGameWindow = new SaveGameWindow();
	setAllButtonsEnabled(true);
	addObject(m_saveGameWindow);
}

void SaveGameScreen::execOnExit(const Screen *nextScreen)
{
	delete m_title;
}

// <<< functions for agents >>>

// yes or no forms
void SaveGameScreen::onNo()
{
	m_yesOrNoForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onOverwriteSaveGame()
{
	m_yesOrNoForm = nullptr;
	if (m_characterCore->save(m_saveGameWindow->getChosenFilename(), m_saveGameWindow->getChosenSaveName()))
	{
		setTooltipText(g_textProvider->getText("GameSaved"), CENDRIC_COLOR_LIGHT_PURPLE, true);;
	}
	else
	{
		g_logger->logError("SaveGameScreen", "Savegame could not be saved");
		setTooltipText(g_textProvider->getText("OperationFailed"), sf::Color::Red, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onDeleteSaveGame()
{
	m_yesOrNoForm = nullptr;
	if (remove(m_saveGameWindow->getChosenFilename().c_str()) == 0)
	{
		setTooltipText(g_textProvider->getText("SavegameDeleted"), CENDRIC_COLOR_LIGHT_PURPLE, true);
	}
	else
	{
		g_logger->logError("SaveGameScreen", "Savegame could not be deleted");
		setTooltipText(g_textProvider->getText("OperationFailed"), sf::Color::Red, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

// new save game form
void SaveGameScreen::onCancel()
{
	m_newSaveGameForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onNewSaveGame()
{
	string name = m_newSaveGameForm->getSavegameName();
	string cleanedName = name;

	string illegalChars = "\\/:?\"<>|*%.";
	for (auto& it : cleanedName)
	{
		bool found = illegalChars.find(it) != string::npos;
		if (found) it = ' ';
	}

	std::string file = "saves/" + to_string(time(nullptr)) + cleanedName + ".sav";

	m_newSaveGameForm = nullptr;
	if (m_characterCore->save(file, name))
	{
		setTooltipText(g_textProvider->getText("GameSaved"), CENDRIC_COLOR_LIGHT_PURPLE, true);
	}
	else
	{
		g_logger->logError("SaveGameScreen", "Savegame could not be created");
		setTooltipText(g_textProvider->getText("OperationFailed"), sf::Color::Red, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}