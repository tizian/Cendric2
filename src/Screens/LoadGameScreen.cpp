#include "Screens/LoadGameScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

LoadGameScreen::LoadGameScreen(CharacterCore* core) : Screen(core)
{
}

Screen* LoadGameScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}
	else if (m_loadGame)
	{
		return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
	}
	if (m_yesOrNoForm == nullptr && (m_loadButton->isClicked() || m_saveGameWindow->isChosen()))
	{
		if (m_characterCore == nullptr)
		{
			// load a savegame
			m_characterCore = new CharacterCore();
			if (!(m_characterCore->load(m_saveGameWindow->getChosenFilename())))
			{
				string errormsg = string(m_saveGameWindow->getChosenFilename()) + ": save file corrupted!";
				g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
			}
			return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
		}
		else
		{
			m_newCharacterCore = new CharacterCore();
			if (!(m_newCharacterCore->load(m_saveGameWindow->getChosenFilename())))
			{
				string errormsg = string(m_saveGameWindow->getChosenFilename()) + ": save file corrupted!";
				g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
			}
			m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
			m_yesOrNoForm->setMessage("QuestionLoadGame");
			m_yesOrNoForm->setOnNoClicked(std::bind(&LoadGameScreen::onNoPressed, this));
			m_yesOrNoForm->setOnYesClicked(std::bind(&LoadGameScreen::onLoadGamePressed, this));
			addObject(GameObjectType::_Form, m_yesOrNoForm);
			setAllButtonsEnabled(false);
		}
	}
	updateObjects(GameObjectType::_Window, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	deleteDisposedObjects();
	return this;
}

void LoadGameScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderObjects(GameObjectType::_Window, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
}

void LoadGameScreen::execOnEnter(const Screen *previousScreen)
{
	// text
	m_title = new BitmapText(g_textProvider->getText("LoadGame"));
	m_title->setCharacterSize(25);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	// add buttons
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(GameObjectType::_Button, m_backButton);

	m_loadButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	m_loadButton->setText("Load");
	addObject(GameObjectType::_Button, m_loadButton);

	// savegame window
	m_saveGameWindow = new SaveGameWindow();
	if (m_saveGameWindow->getChosenFilename().empty())
	{
		m_loadButton->setEnabled(false);
	}
	addObject(GameObjectType::_Window, m_saveGameWindow);
}

void LoadGameScreen::execOnExit(const Screen *nextScreen)
{
	delete m_title;
	delete m_newCharacterCore;
}

// <<< agents for yes or no form >>>

void LoadGameScreen::onNoPressed()
{
	m_yesOrNoForm = nullptr;
	delete m_newCharacterCore;
	m_newCharacterCore = nullptr;
	setAllButtonsEnabled(true);
}

void LoadGameScreen::onLoadGamePressed()
{
	m_yesOrNoForm = nullptr;
	delete m_characterCore;
	m_characterCore = m_newCharacterCore;
	m_newCharacterCore = nullptr;
	m_loadGame = true;
}