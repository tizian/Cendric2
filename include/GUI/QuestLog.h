#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "Window.h"
#include "GUI/QuestDescriptionWindow.h"
#include "GUI/TabBar.h"
#include "GUI/QuestMarker.h"
#include "GUI/SelectableWindow.h"
#include "GUI/ScrollWindow.h"
#include "Enums/EnumNames.h"

class QuestDescriptionWindow;
class ScrollBar;
class ScrollHelper;
class WorldInterface;

// a quest marker on the quest log
class LogQuestMarker final : public QuestMarker {
public:
	LogQuestMarker(const QuestData& questData, WorldInterface* interface);

	void onLeftClick() override;
	void onRightClick() override;

private:
	void init();
	void jumpToQuest();
	void execSetActive() override;
	WorldInterface* m_interface;
};

// a quest entry in the quest log
class QuestEntry final : public ScrollEntry {
public:
	QuestEntry(const QuestData& data, WorldInterface* interface, bool isActiveQuest);
	~QuestEntry();

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	
	const std::string& getQuestID() const;

private:
	void setupQuestMarker(bool isActiveQuest, WorldInterface* interface);
	void updateColor() override;

	BitmapText m_name;

	QuestData m_data;
	QuestMarker* m_questMarker = nullptr;
};

// the quest log, as displayed in a level or a map
// it takes its information directly from the character core
class QuestLog final : public SelectableWindow, public ScrollWindow {
public:
	QuestLog(WorldInterface* interface);
	~QuestLog();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void reload();
	void notifyJumpToQuest(const std::string& questId);

public:
	static const int MAX_ENTRY_LENGTH_CHARACTERS;

	static const float TOP;
	static const float LEFT;
	static const float WIDTH;

private:
	CharacterCore* m_core;
	WorldInterface* m_interface;

	bool m_isVisible = false;

	void init();
	void clearAllEntries();
	void updateSelectableWindow();
	void updateTabBar(const sf::Time& frameTime);

	BitmapText m_title;
	BitmapText m_emptyText;

	TabBar* m_tabBar;

	std::vector<ScrollEntry*> m_startedQuests;
	std::vector<ScrollEntry*> m_completedQuests;
	std::vector<ScrollEntry*> m_failedQuests;

	QuestState m_currentTab;
	std::string m_selectedQuestID = "";

	void selectTab(QuestState state);

	QuestDescriptionWindow* m_descriptionWindow = nullptr;
	void showDescription(const std::string& questID);
	void hideDescription();

protected:
	void updateWindowSelected() override;
	bool isEntryInvisible(const ScrollEntry* entry) const override;
	void execEntrySelected(const ScrollEntry* entry) override;

	int getEntryCount() override { return ENTRY_COUNT; }
	float getLeft() override { return LEFT + SCROLL_WINDOW_LEFT; }
	float getTop() override { return TOP + SCROLL_WINDOW_TOP; }
	float getWindowMargin() override { return WINDOW_MARGIN; }
	float getWidth() override { return SCROLL_WINDOW_WIDTH; }

private:
	static const int ENTRY_COUNT;
	static const float MAX_ENTRY_LENGTH;

	static const float WINDOW_MARGIN;
	static const sf::Vector2f BUTTON_SIZE;

	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;
};