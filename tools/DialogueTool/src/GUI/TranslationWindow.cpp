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
	ImGui::End();
}
