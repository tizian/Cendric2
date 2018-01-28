#pragma once

#include "global.h"

class Dialogue;
class LogWindow;

// describes the configuration that can be modified by the user via options.
// it stores mainly paths.
struct Configuration final {
	std::string npcFolder = "res/npc";
	std::string sqlFolder = "db/insert_scripts/dialogue";
	std::string dialogueFolder = "res/convenience/dialogue";
	std::string parseFileIn;
	std::string parseFileOut;
};

// current state of the application. It holds the configuration and the current dialogue.
// singleton + global
class ApplicationState final {
public:
	ApplicationState();
	~ApplicationState();

	void load();
	
	void loadDialogue(const std::string& dialogueName);
	void setConfiguration(const Configuration& configuration);
	void setLogWindow(LogWindow* window);
	void setLogMessage(const std::string& logMessage, const sf::Color& color);
	// returns the current dialogue of the application. May return null.
	Dialogue* getDialogue() const;
	Configuration& getConfiguration();
	
	bool isQuitRequested() const;

	// quits the application
	void requestQuit();
	// tries to start a new dialogue, returns whether this is possible (or if the npc with that id already exists)
	bool requestNewDialogue(const std::string& npcID);

private:
	Dialogue* m_dialogue = nullptr;
	Configuration m_configuration;
	bool m_isQuitRequested = false;
	LogWindow* m_logWindow = nullptr;
};