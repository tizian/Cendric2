#include "GUI/LogWindow.h"
#include "ApplicationState.h"

LogWindow::LogWindow() {
}

void LogWindow::addLog(const std::string& logMessage, const sf::Color& color) {
	m_logs.push_back(std::pair<std::string, sf::Color>(logMessage, color));
}

void LogWindow::update() {
	ImGui::Begin("Log");
	for (auto& log : m_logs) {
		ImGui::PushStyleColor(ImGuiCol_Text, log.second);
		ImGui::TextWrapped(log.first.c_str());
		ImGui::PopStyleColor();
		ImGui::Spacing();
	}
	// scroll to bottom of window
	ImGui::SetScrollHere(1.f);
	ImGui::End();
}
