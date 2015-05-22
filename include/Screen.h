#pragma once

#include <vector>

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "CharacterCore.h"

#include "Enums/ScreenID.h"

class ScreenManager;

class Screen
{
public:
	Screen(CharacterCore* core);

	virtual Screen* update(const sf::Time& frameTime) = 0;
	virtual void render(sf::RenderTarget& renderTarget) = 0;

	// initializes the m_object vector. called by ALL subclasses
	void onEnter(const Screen* previousScreen);
	virtual void execOnEnter(const Screen* previousScreen);
	// deletes all objects. called by ALL subclasses (screen manager)
	void onExit(const Screen* nextScreen);
	virtual void execOnExit(const Screen* nextScreen);

	// adds an object of type 'type' to the screen.
	void addObject(GameObjectType type, GameObject* object);

	// gets the vector with the objects of type 'type'
	std::vector<GameObject*>* getObjects(GameObjectType type);

	// getter for the tooltip text
	const sf::Text* getTooltipText() const;
	// gets the character core that is needed by each screen
	CharacterCore* getCharacterCore() const;

	// sets the tooltip text to 'text' and display it at this position (relative to the tooltip view)
	void setTooltipText(const std::string& text, const sf::Vector2f& position, const sf::Color& color);

	// sets the view to the standard view, which means the upper part of the window, the game view, without the tooltip bar.
	void setViewToStandardView(sf::RenderTarget& target) const;

	// sets the view to show only the tooltip bar. used before drawing anything there.
	void setViewToTooltipView(sf::RenderTarget& target) const;

	// renders the tooltip text in 'm_tooltipText'
	void renderTooltipText(sf::RenderTarget& target) const;

	// clears the tooltip text in 'm_tooltipText'
	void clearTooltipText();

protected:
	// deletes all objects marked as 'disposed'
	void deleteDisposedObjects();
	// deletes all objects
	void deleteAllObjects();
	// deletes all objects of type 'type'
	void deleteObjects(GameObjectType type);
	// updates all objects of type 'type'
	void updateObjects(GameObjectType type, sf::Time frameTime);
	// render all objects of type 'type'
	void renderObjects(GameObjectType type, sf::RenderTarget& renderTarget);
	
	CharacterCore* m_characterCore;

private:
	std::vector<std::vector<GameObject*>> m_objects;
	sf::Text m_tooltipText;
	
};