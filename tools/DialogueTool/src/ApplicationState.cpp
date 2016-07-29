#include "ApplicationState.h"
#include "FileIO/ConfigurationIO.h"
#include "Dialogue.h"

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

bool ApplicationState::isQuitRequested() const {
	return m_isQuitRequested;
}

void ApplicationState::requestQuit() {
	m_isQuitRequested = true;
}

void ApplicationState::requestNewDialogue() {
	// TODO: yes or no form
	delete m_dialogue;
	m_dialogue = nullptr;
}