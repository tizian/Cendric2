#include "GUI/ErrorWindow.h"
#include "ApplicationState.h"

ErrorWindow::ErrorWindow() {

}

void ErrorWindow::addError(const std::string& errorMessage) {
	m_errors += "\n\n" + errorMessage;
}

void ErrorWindow::update() {
	ImGui::Begin("Errors");
	ImGui::TextWrapped(m_errors.c_str());
	ImGui::End();
}
