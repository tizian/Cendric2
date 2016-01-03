#include "GUI/SaveGameWindow.h"
#include "FileIO/CharacterCoreReader.h"

#include <sstream>

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(40.f, 30.f);
const sf::FloatRect BOX = sf::FloatRect((WINDOW_WIDTH - 900.f) / 2, 75.f, 900.f, 500.f);
const float COLUMN_WIDTH = 300.f;
const int CHAR_SIZE = 12;
const float LINE_PITCH = 20;
const string SAVE_GAME_FOLDER = "saves/";
const string QUICKSAVE_NAME = "quicksave.sav";

inline bool ends_with(std::string const & value, std::string const & ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

SaveGameWindow::SaveGameWindow() : Window(BOX,
	WindowOrnamentStyle::SIMPLE,
	CENDRIC_COLOR_DARK_PURPLE,
	sf::Color(0, 0, 0, 100), sf::Color::White),
	m_maxShowableEntries(static_cast<int>((BOX.height - 2 * TEXT_OFFSET.x) / LINE_PITCH)) {
	reload();
	m_arrowUp.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_GUI_arrow));
	m_arrowUp.setOrigin(sf::Vector2f(m_arrowUp.getLocalBounds().width / 2, m_arrowUp.getLocalBounds().height / 2));
	m_arrowUp.setPosition(sf::Vector2f(BOX.left + 20.f, BOX.top + 50.f));
	m_arrowDown.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_GUI_arrow));
	m_arrowDown.setOrigin(sf::Vector2f(m_arrowDown.getLocalBounds().width / 2, m_arrowDown.getLocalBounds().height / 2));
	m_arrowDown.setScale(sf::Vector2f(1.f, -1.f));
	m_arrowDown.setPosition(sf::Vector2f(BOX.left + 20.f, BOX.top + BOX.height - 50.f));
}

SaveGameWindow::~SaveGameWindow() {
	g_resourceManager->deleteResource(ResourceID::Texture_GUI_arrow);
}

void SaveGameWindow::reload() {
	m_entries.clear();
	m_isChosen = false;

	DIR *dir;
	struct dirent *de;

	dir = opendir(SAVE_GAME_FOLDER.c_str());
	int nr = 0;
	while (dir) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			// we don't consider directory folders
			continue;
		}
		if (!ends_with(string(de->d_name), ".sav")) {
			// we only consider .sav files in this folder
			g_logger->logWarning("SaveGameWindow", "There is a file of the wrong type in the savegame folder: " + string(de->d_name));
			continue;
		}
		SaveGameEntry entry;
		string location = SAVE_GAME_FOLDER + string(de->d_name);
		if (!entry.load(location.c_str())) {
			g_logger->logError("SaveGameWindow", "Could not load savegame " + location);
			continue;
		}
		entry.init(sf::Vector2f(TEXT_OFFSET.x, TEXT_OFFSET.y + nr * LINE_PITCH));
		entry.deselect();
		nr++;
		m_entries.push_back(entry);
	}
	closedir(dir);
	if (m_entries.size() > 0) {
		m_chosenEntry = 0;
		m_entries[m_chosenEntry].select();
		m_topEntry = 0;
		m_bottomEntry = std::min(static_cast<int>(m_entries.size()) - 1, m_maxShowableEntries - 1);
	}
}

bool SaveGameWindow::isChosen() {
	bool wasChosen = m_isChosen;
	m_isChosen = false;
	return wasChosen;
}

void SaveGameWindow::update(const sf::Time& frameTime) {
	if (!m_entries.empty() && m_isEnabled) {
		int oldEntry = m_chosenEntry;
		if (!updateScrolling(frameTime)) {
			for (int i = m_topEntry; i < m_bottomEntry + 1; i++) {
				m_entries[i].update(frameTime);
				if (m_entries[i].isClicked()) {
					if (i == m_chosenEntry) {
						// a chosen option was clicked again
						m_isChosen = true;
					}
					m_chosenEntry = i;
				}
			}
		}

		if (oldEntry != m_chosenEntry) {
			m_entries[oldEntry].deselect();
			m_entries[m_chosenEntry].select();
		}
		if (g_inputController->isKeyJustPressed(Key::Confirm) || g_inputController->isKeyJustPressed(Key::Interact)) {
			m_isChosen = true;
		}
	}
}

bool SaveGameWindow::updateScrolling(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		m_chosenEntry = max(m_chosenEntry - 1, 0);
		if (m_chosenEntry < m_topEntry) {
			scrollUp();
		}
		m_upActiveTime = frameTime;
		return true;
	}
	if (g_inputController->isKeyJustPressed(Key::Down)) {
		m_chosenEntry = min(m_chosenEntry + 1, static_cast<int>(m_entries.size()) - 1);
		if (m_chosenEntry > m_bottomEntry) {
			scrollDown();
		}
		m_downActiveTime = frameTime;
		return true;
	}
	if (m_upActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Up)) {
			m_upActiveTime += frameTime;
		}
		else {
			m_upActiveTime = sf::Time::Zero;
			return false;
		}
	}
	if (m_downActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Down)) {
			m_downActiveTime += frameTime;
		}
		else {
			m_downActiveTime = sf::Time::Zero;
			return false;
		}
	}
	m_timeSinceTick += frameTime;
	if (m_timeSinceTick < SCROLL_TICK_TIME) return false;
	if (m_upActiveTime > SCROLL_TIMEOUT) {
		m_chosenEntry = max(m_chosenEntry - 1, 0);
		m_timeSinceTick = sf::Time::Zero;
		if (m_chosenEntry < m_topEntry) {
			scrollUp();
		}
		return true;
	}
	if (m_downActiveTime > SCROLL_TIMEOUT) {
		m_chosenEntry = min(m_chosenEntry + 1, static_cast<int>(m_entries.size()) - 1);
		m_timeSinceTick = sf::Time::Zero;
		if (m_chosenEntry > m_bottomEntry) {
			scrollDown();
		}
		return true;
	}
	return false;
}

void SaveGameWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	for (int i = m_topEntry; i < m_bottomEntry + 1; i++) {
		m_entries[i].render(renderTarget);
	}
	if (m_topEntry > 0) {
		renderTarget.draw(m_arrowUp);
	}
	if (m_entries.size() - 1 > m_bottomEntry) {
		renderTarget.draw(m_arrowDown);
	}
}

std::string SaveGameWindow::getChosenFilename() const {
	if (m_entries.empty()) {
		return "";
	}
	return m_entries[m_chosenEntry].getFilename();
}

void SaveGameWindow::setEnabled(bool value) {
	m_isEnabled = value;
}

std::string SaveGameWindow::getChosenSaveName() const {
	if (m_entries.empty()) {
		return "";
	}
	return m_entries[m_chosenEntry].getSaveName();
}

void SaveGameWindow::scrollUp() {
	m_topEntry = m_topEntry - 1;
	m_bottomEntry = m_topEntry + m_maxShowableEntries - 1;
	for (auto& it : m_entries) {
		it.setPositionY(it.getPosition().y + LINE_PITCH);
	}
}

void SaveGameWindow::scrollDown() {
	m_bottomEntry = m_bottomEntry + 1;
	m_topEntry = m_bottomEntry - m_maxShowableEntries + 1;
	for (auto& it : m_entries) {
		it.setPositionY(it.getPosition().y - LINE_PITCH);
	}
}

// <<< SaveGameEntry >>>

SaveGameEntry::SaveGameEntry() {
	m_dateSaved.setCharacterSize(CHAR_SIZE);
	m_dateSaved.setColor(sf::Color::White);
	m_name.setCharacterSize(CHAR_SIZE);
	m_name.setColor(sf::Color::White);
	m_timePlayed.setCharacterSize(CHAR_SIZE);
	m_timePlayed.setColor(sf::Color::White);
}

bool SaveGameEntry::load(const std::string& filename) {
	CharacterCoreReader reader;
	CharacterCoreData data;
	if (!reader.readCharacterCore(filename.c_str(), data)) {
		g_logger->logError("SaveGameEntry", "Could not load data from file: " + std::string(filename));
		return false;
	}

	m_filename = filename;

	// format date saved
	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&data.dateSaved));
	m_dateSaved.setString(string(buff));
	m_name.setString(data.saveGameName);

	// format time played
	int secondsPlayed = static_cast<int>(data.timePlayed.asSeconds());
	int hoursPlayed = secondsPlayed / 3600;
	int minutesPlayed = (secondsPlayed / 60) % 60;
	secondsPlayed = (secondsPlayed % 60);
	string stringHours = hoursPlayed > 0 ? to_string(hoursPlayed) + " h - " : "";
	string stringMinutes = minutesPlayed > 0 ? to_string(minutesPlayed) + " m - " : "";
	string stringSeconds = to_string(secondsPlayed) + " s";
	string formattedTime = stringHours + stringMinutes + stringSeconds;
	m_timePlayed.setString(formattedTime);
	return true;
}

void SaveGameEntry::init(const sf::Vector2f& pos) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, BOX.width, LINE_PITCH));
	setPosition(sf::Vector2f(pos.x + BOX.left, pos.y + BOX.top));
}

const std::string& SaveGameEntry::getFilename() const {
	return m_filename;
}

const std::string SaveGameEntry::getSaveName() const {
	return m_name.getString();
}

void SaveGameEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
	m_timePlayed.setPosition(sf::Vector2f(pos.x + COLUMN_WIDTH, pos.y));
	m_dateSaved.setPosition(sf::Vector2f(pos.x + 2 * COLUMN_WIDTH, pos.y));
}

void SaveGameEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
	renderTarget.draw(m_timePlayed);
	renderTarget.draw(m_dateSaved);
}

void SaveGameEntry::onLeftClick() {
	m_isClicked = true;
}

bool SaveGameEntry::isClicked() {
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void SaveGameEntry::select() {
	m_name.setColor(sf::Color::White);
	m_dateSaved.setColor(sf::Color::White);
	m_timePlayed.setColor(sf::Color::White);
	m_isSelected = true;
}

GameObjectType SaveGameEntry::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void SaveGameEntry::deselect() {
	m_name.setColor(CENDRIC_COLOR_GREY);
	m_dateSaved.setColor(CENDRIC_COLOR_GREY);
	m_timePlayed.setColor(CENDRIC_COLOR_GREY);
	m_isSelected = false;
}

bool SaveGameEntry::isSelected() const {
	return m_isSelected;
}
