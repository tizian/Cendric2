#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "GUI/GUIConstants.h"
#include "Structs/NPCBean.h"
#include "Screens/GameScreen.h"
#include "MerchantInterface.h"
#include "Dialogue.h"

class CharacterCore;

class DialogueOption : public GameObject {
public:
	DialogueOption(std::string text, int nr);
	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftClick() override;
	void select();
	void deselect();
	// query with side effects (unclicks automatically) (bertrand would kill me)
	bool isClicked();
	bool isSelected() const;
private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	BitmapText m_text;
};

class DialogueWindow : public Window {
public:
	DialogueWindow();
	~DialogueWindow();
	void load(const NPCBean& npcBean, GameScreen* screen);
	void render(sf::RenderTarget& renderTarget) override;
	// returns true as long as the dialogue exists and false as soon as it ends
	bool updateDialogue(const sf::Time frameTime);

	void setNPCTalking(const std::string& text);
	void setCendricTalking(const std::string& text);
	void setDialogueChoice(const std::vector<std::pair<std::string, int>>& choices);
	void setNPCTrading(const std::string& text);
	void setPosition(const sf::Vector2f& pos) override;

private:
	void setNPC(const NPCBean& bean);
	void setDialogue(const std::string& dialogueID, GameScreen* screen);
	std::vector<DialogueOption> m_options;
	int m_chosenOption = 0;
	Dialogue* m_dialogue = nullptr;
	BitmapText* m_dialogueText = nullptr;
	BitmapText* m_speakerText = nullptr;
	MerchantInterface* m_merchantInterface = nullptr;
	sf::Sprite m_speakerSprite;
	std::string m_npcName;
	std::string m_npcID;
	sf::IntRect m_npcTexturePosition;
	const sf::IntRect m_cendricTexturePosition = sf::IntRect(0, 0, 250, 250);
	const std::string m_cendricName = "Cendric";
};