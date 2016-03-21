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

class CookingOption : public virtual GameObject {
public:
	CookingOption(const std::string& itemID, const std::string& cookedItemID, int count, int nr);
	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftClick() override;
	void select();
	void deselect();
	// query with side effects (unclicks automatically) (bertrand would kill me)
	bool isClicked();
	bool isSelected() const;
	const std::string& getItemID() const;
private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	std::string m_itemID;
	BitmapText m_text;
};

class MapScreen;

class CookingWindow : public Window {
public:
	CookingWindow(MapScreen* screen);
	~CookingWindow();
	
	void render(sf::RenderTarget& renderTarget) override;
	// returns true as long as the dialogue exists and false as soon as it ends
	bool updateWindow(const sf::Time frameTime);
	
	void setPosition(const sf::Vector2f& pos) override;

private:
	void reload();
	void cookItem(const std::string& itemID);
	std::vector<CookingOption> m_options;
	BitmapText m_title;
	int m_chosenOption = 0;
	MapScreen* m_screen = nullptr;
	
	sf::Sprite m_cookingSprite;
};