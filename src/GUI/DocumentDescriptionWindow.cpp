#include "GUI/DocumentDescriptionWindow.h"

using namespace std;

const float DocumentDescriptionWindow::WIDTH = 340.f;

DocumentDescriptionWindow::DocumentDescriptionWindow(const Item& item) : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	WindowOrnamentStyle::FANCY,
	COLOR_LIGHT_BROWN, // back
	COLOR_DARK_BROWN) // ornament 
{
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_BLACK);
	m_text.setString(g_textProvider->getCroppedText(
		item.getID(), "item_text", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	float height = 4 * GUIConstants::TEXT_OFFSET;
	height += m_text.getLocalBounds().height;
	setHeight(height);
}

void DocumentDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	float y = position.y + 2 * GUIConstants::TEXT_OFFSET;
	m_text.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
}

void DocumentDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	renderTarget.draw(m_text);
}


