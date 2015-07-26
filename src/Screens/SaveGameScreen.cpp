#include "Screens/SaveGameScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

SaveGameScreen::SaveGameScreen(CharacterCore* core) : Screen(core)
{
	// precondition: character core can't be nullptr here.
}

void SaveGameScreen::setAllButtonsEnabled(bool value)
{
	Screen::setAllButtonsEnabled(value);
	m_saveButton->setEnabled(value && !m_saveGameWindow->getChosenFilename().empty());
}

Screen* SaveGameScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}
	else if (m_yesOrNoForm == nullptr && m_newSaveGameForm == nullptr && (m_saveButton->isClicked() || m_saveGameWindow->isChosen()))
	{
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
		m_yesOrNoForm->setMessage("QuestionOverwriteSaveGame");
		m_yesOrNoForm->setOnNoClicked(std::bind(&SaveGameScreen::onNoPressed, this)); 
		m_yesOrNoForm->setOnYesClicked(std::bind(&SaveGameScreen::onOverwriteSaveGamePressed, this));
		addObject(GameObjectType::_Form, m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
	else if (m_yesOrNoForm == nullptr && m_newSaveGameForm == nullptr && m_newSaveGameButton->isClicked())
	{
		m_newSaveGameForm = new NewSaveGameForm(sf::FloatRect(400, 350, 450, 200));
		m_newSaveGameForm->setOnOkClicked(std::bind(&SaveGameScreen::onNewSaveGamePressed, this));
		m_newSaveGameForm->setOnCancelClicked(std::bind(&SaveGameScreen::onCancelPressed, this));
		addObject(GameObjectType::_Form, m_newSaveGameForm);
		setAllButtonsEnabled(false);
	}
	updateObjects(GameObjectType::_Window, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);
	deleteDisposedObjects();
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
	m_title->setCharacterSize(25);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	// add buttons
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(GameObjectType::_Button, m_backButton);

	m_saveButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	m_saveButton->setText("Save");
	addObject(GameObjectType::_Button, m_saveButton);

	m_newSaveGameButton = new Button(sf::FloatRect((WINDOW_WIDTH - 200) / 2, WINDOW_HEIGHT - 100, 200, 50));
	m_newSaveGameButton->setText("New");
	addObject(GameObjectType::_Button, m_newSaveGameButton);

	// savegame window
	m_saveGameWindow = new SaveGameWindow();
	if (m_saveGameWindow->getChosenFilename().empty())
	{
		m_saveButton->setEnabled(false);
	}
	addObject(GameObjectType::_Window, m_saveGameWindow);
}

void SaveGameScreen::execOnExit(const Screen *nextScreen)
{
	delete m_title;
}

// <<< functions for agents >>>

// yes or no forms
void SaveGameScreen::onNoPressed()
{
	m_yesOrNoForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onOverwriteSaveGamePressed()
{
	m_yesOrNoForm = nullptr;
	if (m_characterCore->save(m_saveGameWindow->getChosenFilename(), m_saveGameWindow->getChosenSaveName()))
	{
		setTooltipText(g_textProvider->getText("GameSaved"), CENDRIC_COLOR_LIGHT_PURPLE, true);;
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onDeleteSaveGamePressed()
{
	// TODO
}

// new save game form
void SaveGameScreen::onCancelPressed()
{
	m_newSaveGameForm = nullptr;
	setAllButtonsEnabled(true);
}

void SaveGameScreen::onNewSaveGamePressed()
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
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}