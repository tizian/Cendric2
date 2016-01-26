#include "GUI/ModifierSlot.h"

#include "ResourceManager.h"
#include "GUI/ModifierDescriptionWindow.h"

using namespace std;

const float ModifierSlot::SIZE = 66.f;
const float ModifierSlot::ICON_OFFSET = 8.f;

ModifierSlot::ModifierSlot(const SpellModifier& modifier) {
	m_spellModifier = modifier;

	m_iconTexture = g_resourceManager->getTexture(ResourceID::Texture_gems);
	if (modifier.type == SpellModifierType::VOID) {
		m_isEmpty = true;
		m_iconTextureRect = sf::IntRect(0, 0, 50, 50);
	}
	else {
		m_iconTextureRect = sf::IntRect((modifier.level - 1) * 50, 50, 50, 50);
		m_iconRect.setFillColor(SpellModifier::getSpellModifierColor(modifier.type));
	}

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_modifier);
	m_borderTextureSelected = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_modifier_selected);

	m_highlightTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_highlight);

	initSlot();
}

ModifierSlot::~ModifierSlot() {
	delete m_descriptionWindow;
}

void ModifierSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	if (m_descriptionWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left, getBoundingBox()->top - m_descriptionWindow->getSize().y - 20.f);
		m_descriptionWindow->setPosition(pos);
	}
}

void ModifierSlot::renderAfterForeground(sf::RenderTarget& target) {
	Slot::renderAfterForeground(target);
	if (m_showDescriptionWindow && m_descriptionWindow != nullptr) {
		m_descriptionWindow->render(target);
		m_showDescriptionWindow = false;
	}
}

void ModifierSlot::onMouseOver() {
	if (m_isEmpty) return;
	if (m_descriptionWindow == nullptr) {
		m_descriptionWindow = new ModifierDescriptionWindow(m_spellModifier);
		sf::Vector2f pos(getBoundingBox()->left, getBoundingBox()->top - m_descriptionWindow->getSize().y - 10.f);
		m_descriptionWindow->setPosition(pos);
	}
	m_showDescriptionWindow = true;
}
