#include "GUI/HintDescriptionWindow.h"

using namespace std;

const float HintDescriptionWindow::WIDTH = 340.f;

HintDescriptionWindow::HintDescriptionWindow(const CharacterCore* core) : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	GUIOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_core = core;

	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(COLOR_WHITE);
	m_titleText.setTextAlignment(TextAlignment::Center);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(COLOR_LIGHT_GREY);

	setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET + CharacterInfo::WIDTH, GUIConstants::TOP));
}

void HintDescriptionWindow::reload(const std::string& hintKey) {
	std::string title = getHintTitle(hintKey);
	title = g_textProvider->getCroppedString(
		title,
		GUIConstants::CHARACTER_SIZE_M,
		static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET));

	m_titleText.setString(title);

	std::string description = getHintDescription(hintKey);
	description = g_textProvider->getCroppedString(
		description,
		GUIConstants::CHARACTER_SIZE_S,
		static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET));

	m_descriptionText.setString(description);

	setPosition(getPosition());
}

void HintDescriptionWindow::show() {
	m_isVisible = true;
}

void HintDescriptionWindow::hide() {
	m_isVisible = false;
}

void HintDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	sf::Vector2f pos(position);
	pos.y += GUIConstants::TEXT_OFFSET;
	pos.x += GUIConstants::TEXT_OFFSET;

	m_titleText.setPosition(position.x + ((WIDTH - m_titleText.getLocalBounds().width) / 2.f), pos.y);
	pos.y += m_titleText.getLocalBounds().height + GUIConstants::TEXT_OFFSET;

	m_descriptionText.setPosition(pos);

	pos.y += GUIConstants::TEXT_OFFSET + m_descriptionText.getLocalBounds().height;

	setHeight(pos.y - position.y);
}

void HintDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
}