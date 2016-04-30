#pragma once

#include "global.h"
#include "InputController.h"

#include "Enums/GameObjectState.h"
#include "Enums/GameObjectType.h"

class Screen;
class GameObjectComponent;

// A game object with position, bounding box, game state that can be added to a screen.
class GameObject {
public:
	GameObject() {}
	virtual ~GameObject();

	// used to update objects before the real updates, called by the screens.
	virtual void updateFirst(const sf::Time& frameTime) {};
	virtual void update(const sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget);
	// used for tooltips and loot windows
	virtual void renderAfterForeground(sf::RenderTarget& renderTarget);
	// gets checked & called in the update loop. default implementation does nothing.
	// checks for the bounding box 
	virtual void onMouseOver();
	// gets checked & called in the update loop. default implementation does nothing.
	// checks for the bounding box 
	// right click is used to interact with game objects
	virtual void onRightClick();
	// left click is used to fire spells / press buttons
	virtual void onLeftClick();
	virtual void onRightJustPressed();
	virtual void onLeftJustPressed();

	void setBoundingBox(const sf::FloatRect& rect);
	virtual void setDebugBoundingBox(const sf::Color& debugColor);
	virtual void setSize(const sf::Vector2f& size);
	virtual void setPosition(const sf::Vector2f& pos);
	virtual void setPositionX(float posX);
	virtual void setPositionY(float posY);
	virtual void setDisposed();
	virtual void setState(GameObjectState state);
	virtual void setScreen(Screen* screen);
	void setInputInDefaultView(bool value);
	virtual void setViewable(bool value);

	const sf::FloatRect* getBoundingBox() const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::Vector2f getCenter() const;
	// is the object currently visible inside this view + margin?
	bool isViewable() const;
	// should this GO be updated?
	bool isUpdatable() const;
	// returns whether the game object should be deleted
	// if this is set, the game object gets deleted in the next game loop
	bool isDisposed() const;
	virtual GameObjectType getConfiguredType() const = 0;
	Screen* getScreen() const;
	GameObjectState getGameObjectState() const;

protected:
	GameObjectState m_state = GameObjectState::VOID;

	Screen* m_screen = nullptr;
	std::vector<GameObjectComponent*> m_components;
	sf::RectangleShape m_debugBox;

	bool m_isDisposed = false;
	bool m_isViewable = true;
	bool m_isAlwaysUpdate = false; // this property can be set to true if this game object should always update

	// holds absolute position as seen from the upper left corner, and size
	sf::FloatRect m_boundingBox;

	bool m_isDebugRendering = false;
	bool m_isInputInDefaultView = false;

protected:
	void addComponent(GameObjectComponent* component);
};