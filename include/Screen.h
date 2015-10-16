#pragma once

#include <vector>

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "CharacterCore.h"

#include "GUI/BitmapText.h"

class ScreenManager;

class Screen {
public:
	Screen(CharacterCore* core);
	virtual ~Screen() {}

	virtual Screen* update(const sf::Time& frameTime) = 0;
	virtual void render(sf::RenderTarget& renderTarget) = 0;

	// initializes the m_object vector. called by ALL subclasses
	void onEnter(const Screen* previousScreen);
	virtual void execOnEnter(const Screen* previousScreen);
	// deletes all objects. called by ALL subclasses (screen manager)
	void onExit(const Screen* nextScreen);
	virtual void execOnExit(const Screen* nextScreen);

	// adds a gameobject to the screen.
	void addObject(GameObject* object);

	// gets the vector with the objects of type 'type'
	std::vector<GameObject*>* getObjects(GameObjectType type);

	// getter for the tooltip text
	const BitmapText* getTooltipText() const;
	// gets the character core that is needed by each screen
	virtual CharacterCore* getCharacterCore() const;

	// sets the tooltip text to 'text' and display it at the tooltip position thats always at the bootom mid of the screen
	// if override is set, this new text will display anyway, regardless of what other text is displaying.
	void setTooltipText(const std::string& text, const sf::Color& color, bool isOverride);
	// updates the tooltip text in 'm_tooltipText'. used so tooltip texts don't get stuck.
	void updateTooltipText(const sf::Time& frameTime);
	// renders the tooltip text in 'm_tooltipText'
	void renderTooltipText(sf::RenderTarget& target) const;
	// sets the tooltip text to the top of the screen instead of the bottom (if top = true, else bottom)
	void setTooltipPositionTop(bool top);
	// the screen manager sees if a screen wants to end the game
	bool isQuitRequested() const;

protected:
	// deletes all objects marked as 'disposed'
	void deleteDisposedObjects();
	// deletes all objects
	void deleteAllObjects();
	// deletes all objects of type 'type'
	void deleteObjects(GameObjectType type);
	// updates all objects of type 'type'
	void updateObjects(GameObjectType type, const sf::Time& frameTime);
	// render all objects of type 'type'
	void renderObjects(GameObjectType type, sf::RenderTarget& renderTarget);
	// render all objects after foreground of type 'type'
	void renderObjectsAfterForeground(GameObjectType type, sf::RenderTarget& renderTarget);

	CharacterCore* m_characterCore = nullptr;
	bool m_requestQuit = false;

protected:
	// enables / disables all buttons on this screen
	virtual void setAllButtonsEnabled(bool value);

private:
	std::vector<std::vector<GameObject*>> m_objects;
	BitmapText m_tooltipText;

	sf::Time m_tooltipTime = sf::Time::Zero;
	bool m_isTooltipTop = false;
};