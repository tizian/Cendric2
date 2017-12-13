#include "GUI/Stopwatch.h"
#include "GlobalResource.h"
#include "GUI/GUIConstants.h"

Stopwatch::Stopwatch() {
	restart();

	m_text.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.5f - 30.f, WINDOW_HEIGHT - 30.f));
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_BAD);
	m_text.setTextStyle(TextStyle::Shadowed);

	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;
}

void Stopwatch::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
}

void Stopwatch::update(const sf::Time& frameTime) {
	// format time played
	int milliSecondsPlayed = static_cast<int>(m_stopwatch.getElapsedTime().asMilliseconds());
	int secondsPlayed = milliSecondsPlayed / 1000;
	milliSecondsPlayed = (milliSecondsPlayed % 1000);
	std::string stringSeconds = std::to_string(secondsPlayed) + "' ";
	std::string stringMilliSeconds = std::to_string(milliSecondsPlayed) + "''";
	std::string formattedTime = stringSeconds + stringMilliSeconds;
	m_text.setString(formattedTime);
}

void Stopwatch::restart() {
	m_stopwatch.restart();
}

GameObjectType Stopwatch::getConfiguredType() const {
	return GameObjectType::_Form;
}
