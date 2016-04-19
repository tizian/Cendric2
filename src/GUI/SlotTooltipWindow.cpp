#include "GUI/SlotTooltipWindow.h"
#include "GUI/GUIConstants.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

const int SlotTooltipWindow::MAX_WIDTH = 150;

SlotTooltipWindow::SlotTooltipWindow() : Window(
	sf::FloatRect(), GUIOrnamentStyle::NONE, 
	sf::Color(0, 0, 0, 70),
	COLOR_WHITE) 
{
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_text.setPosition(getPosition() + TEXT_OFFSET);
}

void SlotTooltipWindow::setText(const std::string& text) {
	std::string cropped = g_textProvider->getCroppedString(text, GUIConstants::CHARACTER_SIZE_S, MAX_WIDTH);
	m_text.setString(cropped);
	setHeight(m_text.getLocalBounds().height + 2 * TEXT_OFFSET.y);
	setWidth(m_text.getLocalBounds().width + 2 * TEXT_OFFSET.x);
}

void SlotTooltipWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	m_text.setPosition(position + TEXT_OFFSET);
}

void SlotTooltipWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	renderTarget.draw(m_text);
}