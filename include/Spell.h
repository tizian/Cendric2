#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Level.h"

class LevelMovableGameObject;

enum class SpellID
{
	Chop,
	Fire,
	Ice,
	Forcefield
};

/* unlike the values loaded in load() these values here are modifiable by crystal modifiers */
struct SpellBean
{
	SpellID id;
	int damage;
	int reflectCount;
	sf::Time cooldown;
	sf::Time maxActiveTime;
	float startVelocity;
	sf::FloatRect boundingBox; // gets only set after load if it is not [0,0,0,0]
};

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:

	// called by the spell manager
	void init(SpellBean& bean);
	void loadSpell(Level* level, LevelMovableGameObject* mob);
	virtual void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;

	// the offset of the spells start position, as seen from the upper mid of the mobs bounding box. The default is the position of the staff head
	virtual const sf::Vector2f getConfiguredPositionOffset() const;
	// returns whether the spell is bound to the main character and will update its position according to the mob. default is false.
	// if this value is set to true, the velocity of a spell has no influence anymore.
	virtual bool getConfiguredIsAttachedToMob() const;
	// if true, the mob will be in its "fighting" state for the duration of the spell (or the fighting animation (cendric)). default is false.
	virtual bool getConfiguredTriggerFightAnimation() const;
	// if true, the spell sprite will be rotated accordingly. default is true.
	virtual bool getConfiguredRotateSprite() const;
	virtual SpellID getConfiguredSpellID() const = 0;
	GameObjectType getConfiguredType() const override;
	sf::Color getConfiguredDebugColor() const override;

	int getDamage();
	sf::Time getActiveTime();

private:
	Level* m_level;
	LevelMovableGameObject* m_mob; // owner
	sf::Time m_activeCoolDown;

	int m_damage;
	int m_reflectCount;
	float m_speed;

	// calculates position according to m_mob
	void calculatePositionAccordingToMob(sf::Vector2f& position) const;
};