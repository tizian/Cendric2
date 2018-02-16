#include "GUI/Stopwatch.h"
#include "GlobalResource.h"
#include "GUI/GUIConstants.h"
#include "CharacterCore.h"

static std::string zeroPadNumber(int number, int pad) {
	std::string nr = std::to_string(number);

	while (nr.size() < pad) {
		nr = "0" + nr;
	}

	return nr;
}

Stopwatch::Stopwatch(const CharacterCore* core) {
	m_core = core;

	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_WHITE);
	m_text.setTextStyle(TextStyle::Shadowed);

	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;

	initBackgroundLayers();
}

Stopwatch::~Stopwatch() {
	delete m_window;
}

void Stopwatch::initBackgroundLayers() {
	auto size = sf::Vector2f(200.f, 30.f);
	auto pos = sf::Vector2f((WINDOW_WIDTH - size.x) * 0.5f, WINDOW_HEIGHT - 38.f);
	auto box = sf::FloatRect(pos.x, pos.y, size.x, size.y);

	m_window = new Window(box,
		GUIOrnamentStyle::NONE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);
}

void Stopwatch::render(sf::RenderTarget& renderTarget) {
	m_window->render(renderTarget);
	renderTarget.draw(m_text);
}

void Stopwatch::update(const sf::Time& frameTime) {

	// format time played
	int milliSecondsPlayed = static_cast<int>(m_core->getTimePlayed().asMilliseconds());
	int hoursPlayed = milliSecondsPlayed / 3600000;
	int minutesPlayed = (milliSecondsPlayed / 60000) % 60;
	int secondsPlayed = (milliSecondsPlayed / 1000) % 60;
	milliSecondsPlayed = (milliSecondsPlayed % 1000);
	std::string stringHours = std::to_string(hoursPlayed) + ":";
	std::string stringMinutes = zeroPadNumber(minutesPlayed, 2) + ":";
	std::string stringSeconds = zeroPadNumber(secondsPlayed, 2) + ":";
	std::string stringMilliSeconds = zeroPadNumber(milliSecondsPlayed, 3);
	std::string formattedTime = stringHours + stringMinutes + stringSeconds + stringMilliSeconds;
	m_text.setString(formattedTime);

	// center text
	m_text.setPosition(sf::Vector2f((WINDOW_WIDTH - m_text.getBounds().width) * 0.5f, WINDOW_HEIGHT - 30.f));
}

GameObjectType Stopwatch::getConfiguredType() const {
	return _Form;
}
