#include "GUI/SaveGameWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "FileIO/CharacterCoreReader.h"
#include "GlobalResource.h"

#include <sstream>

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

const std::string QUICKSAVE_NAME = "quicksave.sav";

const int SaveGameWindow::ENTRY_COUNT = 20;
const float SaveGameWindow::COLUMN_WIDTH = 300.f;

const float SaveGameWindow::WINDOW_MARGIN = 6.f;

const float SaveGameWindow::WIDTH = 4 * WINDOW_MARGIN + 3 * COLUMN_WIDTH + ScrollBar::WIDTH;
const float SaveGameWindow::HEIGHT = ENTRY_COUNT * GUIConstants::CHARACTER_SIZE_M + (ENTRY_COUNT - 1) * GUIConstants::CHARACTER_SIZE_M + 4 * WINDOW_MARGIN;
const float SaveGameWindow::TOP = 75.f;
const float SaveGameWindow::LEFT = 0.5f * (WINDOW_WIDTH - WIDTH);

inline bool ends_with(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

struct sort_date {
	bool operator() (const ScrollEntry* save1, const ScrollEntry* save2) {
		return *dynamic_cast<const SaveGameEntry*>(save1) > *dynamic_cast<const SaveGameEntry*>(save2);
	}
};

SaveGameWindow::SaveGameWindow() {
	m_entries = &m_saveGameEntries;

	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, WIDTH, HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT, TOP));

	// init empty text
	m_emptyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_emptyText.setString(g_textProvider->getText("NoSaves"));
	const sf::FloatRect bounds = m_emptyText.getBounds();
	m_emptyText.setPosition(LEFT + 0.5f * (WIDTH - bounds.width), TOP + 0.5f * (HEIGHT - bounds.height));

	// init scrolling
	m_scrollBar = new ScrollBar(HEIGHT);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + WIDTH - ScrollBar::WIDTH, TOP));

	sf::FloatRect scrollBox(LEFT, TOP, WIDTH, HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	reload();
}

SaveGameWindow::~SaveGameWindow() {
	delete m_scrollBar;
	delete m_scrollHelper;
	CLEAR_VECTOR(m_saveGameEntries);
}

void SaveGameWindow::reload() {
	m_isChosen = false;
	loadSaves(m_saveGameEntries);
	
	if (!m_saveGameEntries.empty()) {
		selectEntry(0);
	} 

	if (m_saveGameEntries.size() < ENTRY_COUNT) {
		m_scrollBar->setScrollPosition(0.f);
	}
}

void SaveGameWindow::loadSaves(std::vector<ScrollEntry*>& entries) {
	for (auto& entry : entries) {
		delete entry;
	}
	entries.clear();

	DIR * dir = opendir((getDocumentsPath(GlobalResource::SAVEGAME_FOLDER)).c_str());
	int nr = 0;
	while (dir) {
		struct dirent * de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			// we don't consider directory folders
			continue;
		}
		if (!ends_with(std::string(de->d_name), ".sav")) {
			// we only consider .sav files in this folder
			g_logger->logWarning("SaveGameWindow", "There is a file of the wrong type in the savegame folder: " + std::string(de->d_name));
			continue;
		}
		auto entry = new SaveGameEntry();
		const auto location = getDocumentsPath(GlobalResource::SAVEGAME_FOLDER) + std::string(de->d_name);
		if (!entry->load(location)) {
			g_logger->logError("SaveGameWindow", "Could not load savegame " + location);
			continue;
		}
		entry->deselect();
		nr++;
		entries.push_back(entry);
	}
	closedir(dir);

	std::sort(entries.begin(), entries.end(), sort_date());
}

bool SaveGameWindow::isChosen() {
	const auto wasChosen = m_isChosen;
	m_isChosen = false;
	return wasChosen;
}

bool SaveGameWindow::isEntryInvisible(const ScrollEntry* entry) const {
	auto const pos = entry->getPosition();
	return pos.y < TOP || pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + HEIGHT;
}

void SaveGameWindow::update(const sf::Time& frameTime) {
	if (!m_isEnabled) return;

	if (m_saveGameEntries.empty()) {
		m_scrollBar->setEnabled(false);
		return;
	}

	m_scrollBar->update(frameTime);

	updateSelection(frameTime);

	for (auto& entry : m_saveGameEntries) {
		if (entry->isClicked()) {
			m_isChosen = true;
		}
	}

	if (g_inputController->isSelected()) {
		m_isChosen = true;
	}

	calculateEntryPositions();
}

void SaveGameWindow::render(sf::RenderTarget& renderTarget) {
	for (auto& entry : m_saveGameEntries) {
		entry->render(m_scrollHelper->texture);
	}

	m_scrollHelper->render(renderTarget);

	renderTarget.draw(m_scrollWindow);
	m_scrollBar->render(renderTarget);

	if (m_saveGameEntries.empty()) {
		renderTarget.draw(m_emptyText);
	}
}

std::string SaveGameWindow::getChosenFilename() const {
	if (m_saveGameEntries.empty()) {
		return "";
	}
	return dynamic_cast<SaveGameEntry*>(m_saveGameEntries[m_selectedEntryId])->getFilename();
}

void SaveGameWindow::setEnabled(bool value) {
	m_isEnabled = value;
}

std::string SaveGameWindow::getChosenSaveName() const {
	if (m_saveGameEntries.empty()) {
		return "";
	}
	return dynamic_cast<SaveGameEntry*>(m_saveGameEntries[m_selectedEntryId])->getSaveName();
}

// <<< SaveGameEntry >>>

SaveGameEntry::SaveGameEntry() {
	m_dateSaved.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_timePlayed.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	updateColor();
}

bool SaveGameEntry::load(const std::string& filename) {
	CharacterCoreReader reader;
	CharacterCoreData data;
	if (!reader.readCharacterCore(filename, data, true)) {
		g_logger->logError("SaveGameEntry", "Could not load data from file: " + std::string(filename));
		return false;
	}

	m_filename = filename;

	// format date saved
	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&data.dateSaved));
	m_dateSaved.setString(std::string(buff));
	m_dateSavedNr = data.dateSaved;
	m_name.setString(data.saveGameName);

	// format time played
	auto secondsPlayed = static_cast<int>(data.timePlayed.asSeconds());
	const auto hoursPlayed = secondsPlayed / 3600;
	const auto minutesPlayed = (secondsPlayed / 60) % 60;
	secondsPlayed = (secondsPlayed % 60);
	const auto stringHours = hoursPlayed > 0 ? std::to_string(hoursPlayed) + " h - " : "";
	const auto stringMinutes = minutesPlayed > 0 ? std::to_string(minutesPlayed) + " m - " : "";
	const auto stringSeconds = std::to_string(secondsPlayed) + " s";
	const auto formattedTime = stringHours + stringMinutes + stringSeconds;
	m_timePlayed.setString(formattedTime);

	m_isHashValid = data.hashValid;
	return true;
}

const std::string& SaveGameEntry::getFilename() const {
	return m_filename;
}

const std::string& SaveGameEntry::getSaveName() const {
	return m_name.getString();
}

void SaveGameEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
	m_timePlayed.setPosition(sf::Vector2f(pos.x + SaveGameWindow::COLUMN_WIDTH, pos.y));
	m_dateSaved.setPosition(sf::Vector2f(pos.x + 2 * SaveGameWindow::COLUMN_WIDTH, pos.y));
}

void SaveGameEntry::setColor(const sf::Color& color) {
	m_name.setColor(color);
	m_timePlayed.setColor(color);
	m_dateSaved.setColor(color);
}

void SaveGameEntry::updateColor() {
	if (isSelected()) {
		setColor(m_isHashValid ? COLOR_WHITE : COLOR_BAD);
	} else {
		setColor(m_isHashValid ? COLOR_GREY : COLOR_ELEMENTAL_INACTIVE);
	}
}

void SaveGameEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
	renderTarget.draw(m_timePlayed);
	renderTarget.draw(m_dateSaved);
}
