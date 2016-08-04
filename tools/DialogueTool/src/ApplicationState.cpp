#include "ApplicationState.h"
#include "FileIO/ConfigurationIO.h"
#include "Dialogue.h"
#include "GUI/LogWindow.h"

#include <iostream>

ApplicationState* g_state;

ApplicationState::ApplicationState() {}

void ApplicationState::load() {
	ConfigurationIO::loadConfiguration();
}

ApplicationState::~ApplicationState() {
	ConfigurationIO::saveConfiguration();
}

Dialogue* ApplicationState::getDialogue() const {
	return m_dialogue;
}

Configuration& ApplicationState::getConfiguration() {
	return m_configuration;
}

void ApplicationState::setConfiguration(const Configuration& configuration) {
	m_configuration = configuration;
}

void ApplicationState::setLogWindow(LogWindow* window) {
	m_logWindow = window;
}

void ApplicationState::setLogMessage(const std::string& logMessage, const sf::Color& color) {
	if (m_logWindow == nullptr) {
		std::cout << logMessage << std::endl;
		return;
	}
	m_logWindow->addLog(logMessage, color);
}

bool ApplicationState::isQuitRequested() const {
	return m_isQuitRequested;
}

void ApplicationState::requestQuit() {
	m_isQuitRequested = true;
}

bool ApplicationState::requestNewDialogue(const std::string& npcID) {
	// TODO: yes or no form
	delete m_dialogue;
	m_dialogue = new Dialogue(npcID);
	return true;
}