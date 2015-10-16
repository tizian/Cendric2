#include "GUI/ModifierDescriptionWindow.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

ModifierDescriptionWindow::ModifierDescriptionWindow(const SpellModifier& modifier) : Window(
	sf::FloatRect(), WindowOrnamentStyle::NONE, 
	sf::Color(0, 0, 0, 70), 
	sf::Color(0, 0, 0, 70), 
	sf::Color::White) 
{
	string description = "";

	description.append(g_textProvider->getText("Type"));
	description.append(": ");
	description.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(modifier.type)));
	description.append("\n");

	description.append(g_textProvider->getText("Level"));
	description.append(": ");
	description.append(to_string(modifier.level));
	description.append("\n");

	m_descriptionText = new BitmapText(description);
	m_descriptionText->setCharacterSize(8);
	m_descriptionText->setPosition(getPosition() + TEXT_OFFSET);
	setHeight(m_descriptionText->getLocalBounds().height + 18.f);
	setWidth(m_descriptionText->getLocalBounds().width + 10.f);
}

ModifierDescriptionWindow::~ModifierDescriptionWindow() {
	delete m_descriptionText;
}

void ModifierDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	if (m_descriptionText != nullptr) {
		m_descriptionText->setPosition(position + TEXT_OFFSET);
	}
}

void ModifierDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	if (m_descriptionText != nullptr) {
		renderTarget.draw(*m_descriptionText);
	}
}