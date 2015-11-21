#include "GUI/ModifierSlot.h"

using namespace std;

const float ModifierSlot::SIDE_LENGTH = 50.f;
const float ModifierSlot::MARGIN = 2.f;

ModifierSlot::ModifierSlot(const SpellModifier& modifier) {
	m_spellModifier = modifier;

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_gems));

	m_inside.setTextureRect(sf::IntRect((modifier.level - 1) * 50, static_cast<int>(modifier.type) * 50, 50, 50));

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_PURPLE);
}

ModifierSlot::ModifierSlot() {
	m_spellModifier.type = SpellModifierType::VOID;
	m_spellModifier.level = 1;

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_gems));
	m_inside.setTextureRect(sf::IntRect((m_spellModifier.level - 1) * 50, static_cast<int>(m_spellModifier.type) * 50, 50, 50));

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_DARK_GREY);
}

ModifierSlot::~ModifierSlot() {
	delete m_descriptionWindow;
}

void ModifierSlot::activate() {
	m_outside.setOutlineColor(sf::Color::Red);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_inside.setFillColor(sf::Color::White);
}

void ModifierSlot::deactivate() {
	m_outside.setOutlineColor(CENDRIC_COLOR_BLACK);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_BLACK);
	m_inside.setFillColor(sf::Color(150, 150, 150));
}

void ModifierSlot::highlight(bool highlight) {
	if (highlight) {
		m_outside.setOutlineColor(sf::Color::Green);
	}
	else {
		m_outside.setOutlineColor(m_isSelected ?
			sf::Color::Red :
			(m_spellModifier.type == SpellModifierType::VOID) ?
		CENDRIC_COLOR_DARK_GREY :
								CENDRIC_COLOR_PURPLE);
	}
}

void ModifierSlot::select() {
	if (m_isSelected || m_spellModifier.type == SpellModifierType::VOID) return;
	m_isSelected = true;
	m_outside.setOutlineColor(sf::Color::Red);
}

void ModifierSlot::deselect() {
	if (!m_isSelected || m_spellModifier.type == SpellModifierType::VOID) return;
	m_isSelected = false;
	m_outside.setOutlineColor(CENDRIC_COLOR_DARK_PURPLE);
}

bool ModifierSlot::isClicked() {
	return m_isClicked;
}

bool ModifierSlot::isRightClicked() {
	return m_isRightClicked;
}

void ModifierSlot::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
	if (m_descriptionWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left, getBoundingBox()->top - m_descriptionWindow->getSize().y - 10.f);
		m_descriptionWindow->setPosition(pos);
	}
}

void ModifierSlot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
}

void ModifierSlot::update(const sf::Time& frameTime) {
	m_isClicked = false;
	m_isRightClicked = false;
	GameObject::update(frameTime);
}

void ModifierSlot::renderAfterForeground(sf::RenderTarget& target) {
	GameObject::renderAfterForeground(target);
	if (m_showDescriptionWindow && m_descriptionWindow != nullptr) {
		m_descriptionWindow->render(target);
		m_showDescriptionWindow = false;
	}
}

void ModifierSlot::onLeftJustPressed() {
	if (m_spellModifier.type == SpellModifierType::VOID) return;
	m_isClicked = true;
	g_inputController->lockAction();
}

void ModifierSlot::onRightClick() {
	if (m_spellModifier.type == SpellModifierType::VOID) return;
	m_isRightClicked = true;
	g_inputController->lockAction();
}

void ModifierSlot::onMouseOver() {
	if (m_spellModifier.type == SpellModifierType::VOID) return;
	if (m_descriptionWindow == nullptr) {
		m_descriptionWindow = new ModifierDescriptionWindow(m_spellModifier);
		sf::Vector2f pos(getBoundingBox()->left, getBoundingBox()->top - m_descriptionWindow->getSize().y - 10.f);
		m_descriptionWindow->setPosition(pos);
	}
	m_showDescriptionWindow = true;
}

GameObjectType ModifierSlot::getConfiguredType() const {
	return GameObjectType::_Interface;
}

const SpellModifier& ModifierSlot::getModifier() const {
	return m_spellModifier;
}

int ModifierSlot::getNr() const {
	return m_nr;
}

void ModifierSlot::setNr(int nr) {
	m_nr = nr;
}

int ModifierSlot::getSpellSlotNr() const {
	return m_spellSlotNr;
}

void ModifierSlot::setSpellSlotNr(int nr) {
	m_spellSlotNr = nr;
}