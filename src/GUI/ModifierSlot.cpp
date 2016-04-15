#include "GUI/ModifierSlot.h"

#include "ResourceManager.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"

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
	initTooltip();
}

void ModifierSlot::initTooltip() {
	if (SpellModifierType::VOID == m_spellModifier.type) {
		m_tooltipWindow.setText("(" + g_textProvider->getText("Empty") + ")");
		return;
	}

	std::string description = "";
	description.append(g_textProvider->getText("Type"));
	description.append(": ");
	description.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(m_spellModifier.type)));
	description.append("\n");

	description.append(g_textProvider->getText("Level"));
	description.append(": ");
	description.append(to_string(m_spellModifier.level));
	description.append("\n");

	m_tooltipWindow.setText(description);
}