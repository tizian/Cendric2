#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Structs/QuestData.h"
#include "GUI/QuestDescriptionWindow.h"
#include "GUI/Button.h"
#include "GUI/TabBar.h"
#include "Enums/EnumNames.h"

class QuestDescriptionWindow;

// a quest entry in the quest log
class QuestEntry : public GameObject {
public:
	QuestEntry(const std::string& questID);

	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftJustPressed() override;
	void select();
	void deselect();
	bool isClicked();
	bool isSelected() const;
	const std::string& getQuestID() const;

private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	BitmapText m_name;

	std::string m_questID;
};

// the quest log, as displayed in a level or a map
// it takes its information directly from the character core
class QuestLog {
public:
	QuestLog(CharacterCore* core);
	~QuestLog();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the quests, depending on the core
	void reload();

	static float WIDTH;

private:
	CharacterCore* m_core;

	bool m_isVisible = false;

	void init();
	void clearAllEntries();

	Window* m_window;

	BitmapText m_title;

	TabBar *m_tabBar;

	std::vector<QuestEntry> m_startedQuests;
	std::vector<QuestEntry> m_completedQuests;
	std::vector<QuestEntry> m_failedQuests;

	QuestState m_currentTab;
	QuestEntry* m_selectedEntry = nullptr;
	std::string m_selectedQuestID = "";

	void selectTab(QuestState state);
	void selectEntry(QuestEntry* selectedEntry);

	QuestDescriptionWindow* m_descriptionWindow = nullptr;
	void showDescription(const std::string& itemID);
	void hideDescription();

	const sf::Vector2f BUTTON_SIZE = sf::Vector2f((WIDTH - 2 * (GUIConstants::TEXT_OFFSET)) / 3.f, 40.f);

	std::map<QuestState, std::vector<QuestEntry>*> m_stateMap;
};