#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Level.h"

enum class SpellID
{
	Chop,
	Fire,
	Ice
};

/* unlike the values loaded in load() these values here are modifiable by crystal modifiers */
struct SpellBean
{
	int damage;
	int reflectCount;
	sf::Time cooldown;
	sf::Time maxActiveTime;
	float startVelocity;
};

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:

	// called by the spell manager
	void init(SpellBean& bean);
	void loadSpell(Level* level, const sf::Vector2f& position, bool isFacingRight);
	virtual void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;

	// the offset of the spells start position, as seen from the upper mid of cendrics bounding box. The default is the position of the staff head
	virtual sf::Vector2f getConfiguredPositionOffset();

	// returns whether the spell object should be deleted
	bool isDisposed();
	void setDisposed();

	int getDamage();

private:
	Level* m_level;
	sf::Vector2f m_nextPosition;
	sf::Time m_activeCoolDown;
	bool m_isDisposed;

	int m_damage;
	int m_reflectCount;
	float m_speed;
};