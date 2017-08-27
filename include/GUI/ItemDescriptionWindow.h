#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "World/Item.h"
#include "World/Weapon.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"
#include "GUI/BitmapTextHolder.h"

class CharacterCore;

class ItemDescriptionWindow : virtual public Window {
public:
	ItemDescriptionWindow();
	virtual ~ItemDescriptionWindow();

	void load(const Item& item, const CharacterCore* core, float goldMultiplier);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	void show();
	void hide();
	bool isVisible() const;

	static const float WIDTH;
	static const float ICON_OFFSET;

protected:
	void clearTexts();
	virtual std::string getGoldText(const Item& item, float goldMultiplier) const;
	virtual std::string getReputationText(const Item& item) const;
	virtual std::string getInteractionText(const Item& item) const;

	bool m_isReputationReached = false;

private:
	void addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines = 0);
	void addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines, int characterSize);
	void addIntComparision(int this_, int other);
	void addCooldownComparision(float this_, float other);
	
	void loadAttributes(const Item& item, const CharacterCore* core, sf::Vector2f& offset);
	void loadDefaultAttributes(const Item& item, sf::Vector2f& offset);
	void loadWeaponAttributes(const Weapon& item, sf::Vector2f& offset);

	void compareAttributes(const Item& item, const Item& comp, sf::Vector2f& offset);
	void compareWeaponAttributes(const Weapon& item, const Weapon& comp, sf::Vector2f& offset);
	void compareDoubleAttributes(const Item& item, const Item& comp1, const Item& comp2, sf::Vector2f& offset);

private:
	std::vector<BitmapTextHolder*> m_texts;

	bool m_isVisible = false;
};