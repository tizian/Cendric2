#pragma once

#include <map>

#include "global.h"
#include "AnimatedSprite.h"
#include "InputController.h"

class Screen;

// this enum is used for animations and used by all game objects
enum class GameObjectState
{
	Idle, // used by nearly every game object
	Idle_up, // used by map main character
	Idle_down, // used by map main character
	Idle_left, // used by map main character
	Idle_right, // used by map main character
	Walking, // used by main character and its weapons
	Walking_up, // used by map main character
	Walking_down, // used by map main character
	Walking_right, // used by map main character
	Walking_left, // used by map main character
	Jumping, // used by main character and its weapons
	Fighting, // used by main character and its weapons
	Frozen, // used by water tile
	Crumbling, // used by crumbly block tile
	Pressed // used by buttons
};

// the type of a game object.
// used by the screen for rendering and update order.
// MAX should not be used as enum types as it is only used for the enum iterator
// undefined should always be the first (=0). It can be used though.
enum GameObjectType
{
	_Undefined,
	_MainCharacter,
	_LevelEquipment, // includes everything cendric can wear
	_Button,
	_Spell,
	_DynamicTile,
	_Enemy,
	_LevelItem,
	_MAX
};

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