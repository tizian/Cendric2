#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/QuestDescriptionWindow.h"
#include "GUI/TabBar.h"
#include "GUI/SlicedSprite.h"
#include "GUI/QuestMarker.h"
#include "Enums/EnumNames.h"

class QuestDescriptionWindow;
class ScrollBar;
class ScrollHelper;
class WorldInterface;

// a quest marker on the quest log
class LogQuestMarker final : public QuestMarker {
public:
	LogQuestMarker(const QuestData& questData, WorldInterface* interface);

	void setActive(bool active) override;

	void onLeftClick() override;
	void onRightClick() override;

private:
	void init();
	void jumpToQuest();
	WorldInterface* m_interface;
};

// a quest entry in the quest log
class QuestEntry final : public GameObject {
public:
	QuestEntry(const QuestData& data, WorldInterface* interface, bool isActiveQuest);
	~QuestEntry();

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftJustPressed() override;
	void onMouseOver() override;
	void select();
	void deselect();
	bool isClicked() const;
	bool isSelected() const;
	const std::string& getQuestID() const;

private:
	void setupQuestMarker(bool isActiveQuest, WorldInterface* interface);

	bool m_isSelected = false;
	bool m_isClicked = false;
	bool m_isMouseover = false;
	BitmapText m_name;

	QuestData m_data;
	QuestMarker* m_questMarker = nullptr;
};

// the quest log, as displayed in a level or a map
// it takes its information directly from the character core
class QuestLog final {
public:
	QuestLog(WorldInterface* interface);
	~QuestLog();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the quests, depending on the core
	void reload();

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
	void calculateEntryPositions();

	Window* m_window;
	
	BitmapText m_title;
	BitmapText m_emptyText;

	TabBar* m_tabBar;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper *m_scrollHelper = nullptr;

	std::vector<QuestEntry*> m_startedQuests;
	std::vector<QuestEntry*> m_completedQuests;
	std::vector<QuestEntry*> m_failedQuests;

	QuestState m_currentTab;
	QuestEntry* m_selectedEntry = nullptr;
	std::string m_selectedQuestID = "";

	void selectTab(QuestState state);
	void selectEntry(QuestEntry* selectedEntry);

	QuestDescriptionWindow* m_descriptionWindow = nullptr;
	void showDescription(const std::string& questID);
	void hideDescription();

	std::map<QuestState, std::vector<QuestEntry*>*> m_stateMap;

	static const int ENTRY_COUNT;
	static const float MAX_ENTRY_LENGTH;

	static const float WINDOW_MARGIN;

	static const sf::Vector2f BUTTON_SIZE;

	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;
};