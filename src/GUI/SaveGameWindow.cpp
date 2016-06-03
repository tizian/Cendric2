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

inline bool ends_with(std::string const & value, std::string const & ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

SaveGameWindow::SaveGameWindow() {
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
}

void SaveGameWindow::reload() {
	m_entries.clear();
	m_isChosen = false;

	DIR *dir;
	struct dirent *de;

	dir = opendir((g_documentsPath + "saves/").c_str());
	int nr = 0;
	while (dir) {
		de = readdir(dir);
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
		SaveGameEntry entry;
		std::string location = g_documentsPath + "saves/" + std::string(de->d_name);
		if (!entry.load(location.c_str())) {
			g_logger->logError("SaveGameWindow", "Could not load savegame " + location);
			continue;
		}
		entry.deselect();
		nr++;
		m_entries.push_back(entry);
	}
	closedir(dir);
	if (m_entries.size() > 0) {
		m_chosenEntry = 0;
		m_entries[m_chosenEntry].select();
	}

	if (m_entries.size() < ENTRY_COUNT) {
		m_scrollBar->setScrollPosition(0.f);
	}
}

bool SaveGameWindow::isChosen() {
	bool wasChosen = m_isChosen;
	m_isChosen = false;
	return wasChosen;
}

void SaveGameWindow::update(const sf::Time& frameTime) {
	if (!m_isEnabled) return;

	if (m_entries.empty()) {
		m_scrollBar->setEnabled(false);
		return;
	}

	m_scrollBar->update(frameTime);

	int oldEntry = m_chosenEntry;

	updateScrolling(frameTime);

	for (size_t i = 0; i < m_entries.size(); ++i) {
		if (i == m_chosenEntry) {
			m_entries[i].setColor(COLOR_WHITE);
		}
		else if (g_inputController->isMouseOver(m_entries[i].getBoundingBox(), true)) {
			m_entries[i].setColor(COLOR_LIGHT_PURPLE);
		}
		else {
			m_entries[i].setColor(COLOR_GREY);
		}
	}

	for (size_t i = 0; i < m_entries.size(); ++i) {
		sf::Vector2f pos = m_entries[i].getPosition();
		if (pos.y < TOP || pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + HEIGHT) continue;
		m_entries[i].update(frameTime);
		if (m_entries[i].isClicked()) {
			if (static_cast<int>(i) == m_chosenEntry) {
				// a chosen option was clicked again
				m_isChosen = true;
			}
			m_chosenEntry = static_cast<int>(i);
		}
	}

	if (oldEntry != m_chosenEntry) {
		m_entries[oldEntry].deselect();
		m_entries[m_chosenEntry].select();
		g_resourceManager->playSound(m_sound, GlobalResource::SOUND_GUI_MENUCURSOR, true);
	}
	if (g_inputController->isSelected()) {
		m_isChosen = true;
	}

	calculateEntryPositions();
}

void SaveGameWindow::calculateEntryPositions() {
	int rows = static_cast<int>(m_entries.size());
	int steps = rows - ENTRY_COUNT + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = 2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M;
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float xOffset = LEFT + 2 * WINDOW_MARGIN;
	float yOffset = TOP + 2 * WINDOW_MARGIN - effectiveScrollOffset;

	for (auto& it : m_entries) {
		it.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void SaveGameWindow::updateScrolling(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		m_chosenEntry = std::max(m_chosenEntry - 1, 0);
		SaveGameEntry& entry = m_entries[m_chosenEntry];
		if (entry.getPosition().y < TOP) {
			m_scrollBar->scroll(-1);
		}
		m_upActiveTime = frameTime;
		return;
	}

	if (g_inputController->isKeyJustPressed(Key::Down)) {
		m_chosenEntry = std::min(m_chosenEntry + 1, static_cast<int>(m_entries.size()) - 1);
		SaveGameEntry& entry = m_entries[m_chosenEntry];
		if (entry.getPosition().y + entry.getSize().y > TOP + HEIGHT) {
			m_scrollBar->scroll(1);
		}
		m_downActiveTime = frameTime;
		return;
	}

	if (m_upActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Up)) {
			m_upActiveTime += frameTime;
		}
		else {
			m_upActiveTime = sf::Time::Zero;
			return;
		}
	}

	if (m_downActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Down)) {
			m_downActiveTime += frameTime;
		}
		else {
			m_downActiveTime = sf::Time::Zero;
			return;
		}
	}

	m_timeSinceTick += frameTime;
	if (m_timeSinceTick < SCROLL_TICK_TIME) return;

	if (m_upActiveTime > SCROLL_TIMEOUT) {
		m_chosenEntry = std::max(m_chosenEntry - 1, 0);
		SaveGameEntry& entry = m_entries[m_chosenEntry];
		m_timeSinceTick = sf::Time::Zero;
		if (entry.getPosition().y < TOP) {
			m_scrollBar->scroll(-1);
		}
		return;
	}

	if (m_downActiveTime > SCROLL_TIMEOUT) {
		m_chosenEntry = std::min(m_chosenEntry + 1, static_cast<int>(m_entries.size()) - 1);
		SaveGameEntry& entry = m_entries[m_chosenEntry];
		m_timeSinceTick = sf::Time::Zero;
		if (entry.getPosition().y + entry.getSize().y > TOP + HEIGHT) {
			m_scrollBar->scroll(1);
		}
		return;
	}
}

void SaveGameWindow::render(sf::RenderTarget& renderTarget) {
	for (size_t i = 0; i < m_entries.size(); ++i) {
		m_entries[i].render(m_scrollHelper->texture);
	}
	m_scrollHelper->render(renderTarget);

	renderTarget.draw(m_scrollWindow);
	m_scrollBar->render(renderTarget);

	if (m_entries.size() == 0) {
		renderTarget.draw(m_emptyText);
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

// <<< SaveGameEntry >>>

SaveGameEntry::SaveGameEntry() {
	m_dateSaved.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_dateSaved.setColor(COLOR_WHITE);
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_name.setColor(COLOR_WHITE);
	m_timePlayed.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_timePlayed.setColor(COLOR_WHITE);
}

bool SaveGameEntry::load(const std::string& filename) {
	CharacterCoreReader reader;
	CharacterCoreData data;
	if (!reader.readCharacterCore(filename.c_str(), data, true)) {
		g_logger->logError("SaveGameEntry", "Could not load data from file: " + std::string(filename));
		return false;
	}

	m_filename = filename;

	// format date saved
	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&data.dateSaved));
	m_dateSaved.setString(std::string(buff));
	m_name.setString(data.saveGameName);

	// format time played
	int secondsPlayed = static_cast<int>(data.timePlayed.asSeconds());
	int hoursPlayed = secondsPlayed / 3600;
	int minutesPlayed = (secondsPlayed / 60) % 60;
	secondsPlayed = (secondsPlayed % 60);
	std::string stringHours = hoursPlayed > 0 ? std::to_string(hoursPlayed) + " h - " : "";
	std::string stringMinutes = minutesPlayed > 0 ? std::to_string(minutesPlayed) + " m - " : "";
	std::string stringSeconds = std::to_string(secondsPlayed) + " s";
	std::string formattedTime = stringHours + stringMinutes + stringSeconds;
	m_timePlayed.setString(formattedTime);
	return true;
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
	m_timePlayed.setPosition(sf::Vector2f(pos.x + SaveGameWindow::COLUMN_WIDTH, pos.y));
	m_dateSaved.setPosition(sf::Vector2f(pos.x + 2 * SaveGameWindow::COLUMN_WIDTH, pos.y));
}

void SaveGameEntry::setColor(const sf::Color& color) {
	m_name.setColor(color);
	m_timePlayed.setColor(color);
	m_dateSaved.setColor(color);
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
	m_name.setColor(COLOR_WHITE);
	m_dateSaved.setColor(COLOR_WHITE);
	m_timePlayed.setColor(COLOR_WHITE);
	m_isSelected = true;
}

GameObjectType SaveGameEntry::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void SaveGameEntry::deselect() {
	m_name.setColor(COLOR_GREY);
	m_dateSaved.setColor(COLOR_GREY);
	m_timePlayed.setColor(COLOR_GREY);
	m_isSelected = false;
}

bool SaveGameEntry::isSelected() const {
	return m_isSelected;
}
