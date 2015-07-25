#include "GUI/SaveGameWindow.h"
#include "FileIO/CharacterCoreReader.h"

#include <sstream>
#include <dirent.h>

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(40.f, 30.f);
const sf::FloatRect BOX = sf::FloatRect((WINDOW_WIDTH - 600.f) / 2, 75.f, 600.f, 500.f);
const float DATE_OFFSET = 300.f;
const int CHAR_SIZE = 12;
const float LINE_PITCH = 20;
const int MAX_CHARS_IN_FILENAME = static_cast<int>(DATE_OFFSET) / CHAR_SIZE - 1;
const string SAVE_GAME_FOLDER = "saves/";
const string QUICKSAVE_NAME = "quicksave.sav";

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

SaveGameWindow::SaveGameWindow() : Window(BOX, WindowOrnamentStyle::LARGE, sf::Color(0, 0, 0, 100), sf::Color(0, 0, 0, 100), sf::Color::White)
{
	DIR *dir;
	struct dirent *de;

	dir = opendir(SAVE_GAME_FOLDER.c_str());
	int nr = 0;
	while (dir)
	{
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR)
		{
			// we don't consider directory folders
			continue;
		}
		if (!ends_with(string(de->d_name), ".sav"))
		{
			// we only consider .sav files in this folder
			g_logger->logWarning("SaveGameWindow", "There is a file of the wrong type in the savegame folder: " + string(de->d_name));
			continue;
		}
		SaveGameEntry entry;
		string location = SAVE_GAME_FOLDER + string(de->d_name);
		if (!entry.load(location.c_str()))
		{
			g_logger->logError("SaveGameWindow", "Could not load savegame " + location);
			continue;
		}
		entry.init(sf::Vector2f(TEXT_OFFSET.x, TEXT_OFFSET.y + nr * LINE_PITCH));
		entry.deselect();
		nr++;
		m_entries.push_back(entry);
	}
	closedir(dir);
	if (m_entries.size() > 0)
	{
		m_chosenEntry = 0;
		m_entries[m_chosenEntry].select();
	}
}

bool SaveGameWindow::isChosen()
{
	bool wasChosen = m_isChosen;
	m_isChosen = false;
	return wasChosen;
}

void SaveGameWindow::update(const sf::Time& frameTime)
{
	if (!m_entries.empty())
	{
		int oldEntry = m_chosenEntry;
		if (g_inputController->isKeyJustPressed(Key::Up))
		{
			m_chosenEntry = max(m_chosenEntry - 1, 0);
		}
		else if (g_inputController->isKeyJustPressed(Key::Down))
		{
			m_chosenEntry = min(m_chosenEntry + 1, static_cast<int>(m_entries.size()) - 1);
		}
		for (int i = 0; i < m_entries.size(); i++)
		{
			m_entries[i].update(frameTime);
			if (m_entries[i].isClicked())
			{
				if (i == m_chosenEntry)
				{
					// a chosen option was clicked again
					m_isChosen = true;
				}
				m_chosenEntry = i;
			}
		}
		if (oldEntry != m_chosenEntry)
		{
			m_entries[oldEntry].deselect();
			m_entries[m_chosenEntry].select();
		}
		if (g_inputController->isKeyJustPressed(Key::Confirm) || g_inputController->isKeyJustPressed(Key::Interact))
		{
			m_isChosen = true;
		}
	}
}

void SaveGameWindow::render(sf::RenderTarget& renderTarget)
{
	Window::render(renderTarget);
	for (auto& it : m_entries)
	{
		it.render(renderTarget);
	}
}

std::string SaveGameWindow::getChosenFilename() const
{
	if (m_entries.empty())
	{
		return "";
	}
	return m_entries[m_chosenEntry].getFilename();
}

// <<< SaveGameEntry >>>

SaveGameEntry::SaveGameEntry()
{
	m_dateSaved.setCharacterSize(CHAR_SIZE);
	m_dateSaved.setColor(sf::Color::White);
	m_name.setCharacterSize(CHAR_SIZE);
	m_name.setColor(sf::Color::White);
}

bool SaveGameEntry::load(const std::string& filename)
{
	CharacterCoreReader reader;
	CharacterCoreData data;
	if (!reader.readCharacterCore(filename.c_str(), data))
	{
		g_logger->logError("SaveGameEntry", "Could not load data from file: " + std::string(filename));
		return false;
	}

	m_filename = filename;

	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&data.dateSaved));
	m_dateSaved.setString(string(buff));
	m_name.setString(data.saveGameName);
	return true;
}

void SaveGameEntry::init(const sf::Vector2f& pos)
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, BOX.width, LINE_PITCH));
	setPosition(sf::Vector2f(pos.x + BOX.left, pos.y + BOX.top));
}

const std::string& SaveGameEntry::getFilename() const
{
	return m_filename;
}

void SaveGameEntry::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
	m_dateSaved.setPosition(sf::Vector2f(pos.x + DATE_OFFSET, pos.y));
}

void SaveGameEntry::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_name);
	renderTarget.draw(m_dateSaved);
}

void SaveGameEntry::onLeftClick()
{
	m_isClicked = true;
}

bool SaveGameEntry::isClicked()
{
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void SaveGameEntry::select()
{
	m_name.setColor(sf::Color::White);
	m_dateSaved.setColor(sf::Color::White);
	m_isSelected = true;
}

GameObjectType SaveGameEntry::getConfiguredType() const
{
	return GameObjectType::_Undefined;
}

void SaveGameEntry::deselect()
{
	m_name.setColor(sf::Color(100, 100, 100));
	m_dateSaved.setColor(sf::Color(100, 100, 100));
	m_isSelected = false;
}

bool SaveGameEntry::isSelected() const
{
	return m_isSelected;
}
