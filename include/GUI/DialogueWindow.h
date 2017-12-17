#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "GUI/GUIConstants.h"
#include "Structs/NPCData.h"
#include "Screens/WorldScreen.h"
#include "Map/MerchantInterface.h"
#include "Map/Dialogue.h"

class CharacterCore;
class NPC;
class ScrollBar;
class ScrollHelper;

enum class DialogueOptionType {
	Default,
	End,
	Trade
};

class DialogueOption final : public GameObject {
public:
	DialogueOption(const ChoiceTranslation& trans, const std::string& dialogueID, DialogueOptionType type, Screen* screen);
	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void setColor(const sf::Color& color);
	void onLeftClick() override;
	void select();
	void deselect();
	bool executeCrafting();
	// query with side effects (unclicks automatically) (bertrand would kill me)
	bool isClicked();
	bool isSelected() const;
	bool isSelectable() const;
private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	bool m_isSelectable = true;
	sf::Text m_text;
	ChoiceTranslation m_translation;
};

class DialogueWindow final : public Window {
public:
	DialogueWindow();
	~DialogueWindow();
	void load(NPC* npc, WorldScreen* screen);
	void render(sf::RenderTarget& renderTarget) override;
	// returns true as long as the dialogue exists and false as soon as it ends
	bool updateDialogue(const sf::Time frameTime);

	void setNPCTalking(const std::string& text);
	void setCendricTalking(const std::string& text);
	void setDialogueChoice(const std::vector<std::pair<ChoiceTranslation, int>>& choices);
	void setNPCTrading(const std::string& text);
	void setPosition(const sf::Vector2f& pos) override;

	NPC* getNPC();

private:
	void setNPC(NPC* npc);
	void setDialogue(const std::string& dialogueID, WorldScreen* screen);

	void calculateEntryPositions();
	void updateScrolling(const sf::Time& frameTime);
	sf::Texture* getCendricTexture();

private:
	std::vector<DialogueOption> m_options;
	int m_chosenOption = 0;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper *m_scrollHelper = nullptr;

	NPC* m_npc;
	sf::Sprite m_speakerSprite;

	std::string m_npcName;
	std::string m_npcID;
	std::string m_dialogueTextID;

	Dialogue* m_dialogue = nullptr;
	sf::Text* m_dialogueText = nullptr;
	BitmapText* m_speakerText = nullptr;
	MerchantInterface* m_merchantInterface = nullptr;

	// the time it waits from key active to scrolling
	const sf::Time SCROLL_TIMEOUT = sf::milliseconds(500);
	sf::Time m_upActiveTime = sf::Time::Zero;
	sf::Time m_downActiveTime = sf::Time::Zero;
	// the time between the scroll-ticks, determines the speed of scrolling
	const sf::Time SCROLL_TICK_TIME = sf::milliseconds(70);
	sf::Time m_timeSinceTick = sf::Time::Zero;
	// the timeout before the next step of a dialogue can be activated
	const sf::Time DIALOGUE_TIMEOUT = sf::milliseconds(500);
	sf::Time m_dialogueTimeout = sf::Time::Zero;

	static const int OPTION_COUNT;

	static const float LEFT_OFFSET;
	static const float RIGHT_OFFSET;
	static const float TEXT_WIDTH;
	static const float WINDOW_MARGIN;

	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;

	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;
};