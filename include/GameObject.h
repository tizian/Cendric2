#pragma once

#include <map>

#include "global.h"
#include "AnimatedSprite.h"
#include "InputController.h"

#include "Enums/GameObjectState.h"
#include "Enums/GameObjectType.h"

class Screen;

// A game object with animations, position, bounding box, game state. 
// subclasses should always call 'load'.
class GameObject 
{
public:
	// \brief loads animations using the resource manager
	// also defines bounding box and sprite offset.
	virtual void load() = 0;
	virtual void update(const sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget);
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

	void addAnimation(GameObjectState state, Animation& animation);

	void setBoundingBox(const sf::FloatRect& rect);
	void setPosition(const sf::Vector2f& pos);
	void setPositionX(float posX);
	void setPositionY(float posY);
	void setDisposed();
	void setCurrentAnimation(const Animation* animation, bool isFlipped);
	void setSpriteOffset(const sf::Vector2f& spriteOffset);
	void setScreen(Screen* screen);
	void setFrameTime(const sf::Time& time);
	// angle is in radian
	void setRotation(float angle);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);

	const Animation* getAnimation(GameObjectState state);
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSpriteOffset() const;
	const sf::FloatRect* getBoundingBox() const;
	const sf::Vector2f getCenter() const;
	// returns whether the game object should be deleted
	// if this is set, the game object gets deleted in the next game loop
	bool isDisposed() const;
	virtual GameObjectType getConfiguredType() const = 0;
	// returns the color in which the debug rect should be rendered. default is transparent.
	virtual sf::Color getConfiguredDebugColor() const;
	Screen* getScreen() const;

protected:
	GameObjectState m_state;
	sf::Vector2f m_nextPosition;
	AnimatedSprite m_animatedSprite;
	Screen* m_screen = nullptr;
	
private:
	bool m_isDisposed = false;

	std::map<GameObjectState, Animation> m_animations;
	sf::Vector2f m_spriteOffset;
	sf::FloatRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;

	void drawBoundingBox(sf::RenderTarget& renderTarget) const;
};