#pragma once

#include <vector>

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "CharacterCore.h"
#include "ScreenOverlays/ScreenOverlay.h"

#include "GUI/BitmapText.h"

class ScreenManager;

class Screen {
public:
	Screen(CharacterCore* core);
	virtual ~Screen();

	void update(const sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget) = 0;

	// initializes the m_object vector. called by ALL subclasses
	void onEnter(Screen* previousScreen);
	virtual void execOnEnter(const Screen* previousScreen);
	// deletes all objects. called by ALL subclasses (screen manager)
	void onExit(Screen* nextScreen);
	virtual void execOnExit(const Screen* nextScreen);

	// adds a gameobject to the screen. This object is only added to the vector in the next iteration
	// to avoid invalidation of vector iterators.
	void addObject(GameObject* object);

	// gets the vector with the objects of type 'type'
	std::vector<GameObject*>* getObjects(GameObjectType type);

	// getter for the tooltip text
	const BitmapText* getTooltipText() const;
	// gets the character core that is needed by each screen
	virtual CharacterCore* getCharacterCore() const;

	// sets the tooltip text to the translated 'textKey' and display it at the tooltip position thats always at the bootom mid of the screen
	// if override is set, this new text will display anyway, regardless of what other text is displaying.
	void setTooltipText(const std::string& textKey, const sf::Color& color, bool isOverride);
	// sets the tooltip text but without translating it
	void setTooltipTextRaw(const std::string& text, const sf::Color& color, bool isOverride);
	// updates the tooltip text in 'm_tooltipText'. used so tooltip texts don't get stuck.
	void updateTooltipText(const sf::Time& frameTime);
	// renders the tooltip text in 'm_tooltipText'
	void renderTooltipText(sf::RenderTarget& target) const;
	// sets the tooltip text to the top of the screen instead of the bottom (if top = true, else bottom)
	void setTooltipPositionTop(bool top);
	// sets the next screen on the screen manager. If the bool property is set for backup,
	// this screen will be backed up and can be returned to by using screenManager->resume()
	void setNextScreen(Screen* nextScreen, bool backupThis = false);
	void setScreenManager(ScreenManager* screenManager);

protected:
	// the update part that is customized per screen
	virtual void execUpdate(const sf::Time& frameTime) = 0;
	// deletes all objects
	void deleteAllObjects();
	// deletes all objects of type 'type'
	void deleteObjects(GameObjectType type);
	// calls the update first method of all objects of type 'type'
	void updateObjectsFirst(GameObjectType type, const sf::Time& frameTime);
	// updates all objects of type 'type'
	void updateObjects(GameObjectType type, const sf::Time& frameTime);
	// depth sorts all objects (y coord asc or desc) of type 'type'
	void depthSortObjects(GameObjectType type, bool asc);
	// render all objects of type 'type'
	void renderObjects(GameObjectType type, sf::RenderTarget& renderTarget);
	// render all objects after foreground of type 'type'
	void renderObjectsAfterForeground(GameObjectType type, sf::RenderTarget& renderTarget);
	// enables / disables all buttons on this screen
	virtual void setAllButtonsEnabled(bool value);

protected:
	CharacterCore* m_characterCore = nullptr;
	ScreenManager* m_screenManager = nullptr;

private:
	// deletes all objects marked as 'disposed'
	void deleteDisposedObjects();
	std::vector<std::vector<GameObject*>> m_objects;
	std::vector<GameObject*> m_toAdd;
	BitmapText m_tooltipText;

	sf::Time m_tooltipTime = sf::Time::Zero;
	bool m_isTooltipTop = false;
};