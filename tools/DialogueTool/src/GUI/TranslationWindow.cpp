#include "GUI/TranslationWindow.h"
#include "Nodes/NodeTranslation.h"

TranslationWindow::TranslationWindow() {
}

void TranslationWindow::setNodeTranslation(NodeTranslation* translation) {
	m_translation = translation;
}

void TranslationWindow::update() {
	if (m_translation == nullptr) return;
	ImGui::Begin("Translation");
	ImGui::InputText("Tag", m_translation->tag, IM_ARRAYSIZE(m_translation->tag));
	ImGui::InputText("English", m_translation->englishTranslation, IM_ARRAYSIZE(m_translation->englishTranslation));
	ImGui::InputText("German", m_translation->germanTranslation, IM_ARRAYSIZE(m_translation->germanTranslation));
	ImGui::InputText("Swiss German", m_translation->swissgermanTranslation, IM_ARRAYSIZE(m_translation->swissgermanTranslation));
	ImGui::Spacing();
	ImGui::InputText("Item ID", m_translation->itemID, IM_ARRAYSIZE(m_translation->itemID));
	ImGui::InputInt("Item Amount", &m_translation->itemAmount);
	ImGui::Checkbox("Crafting", &m_translation->isCrafting);
	ImGui::End();
}
