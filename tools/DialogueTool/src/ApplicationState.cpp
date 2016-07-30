#include "ApplicationState.h"
#include "FileIO/ConfigurationIO.h"
#include "Dialogue.h"
#include "GUI/ErrorWindow.h"

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

void ApplicationState::setErrorWindow(ErrorWindow* window) {
	m_errorWindow = window;
}

void ApplicationState::setError(const std::string& errorMessage) {
	if (m_errorWindow == nullptr) {
		std::cout << errorMessage << std::endl;
		return;
	}
	m_errorWindow->addError(errorMessage);
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