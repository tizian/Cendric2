#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Structs/AttributeData.h"
#include "Structs/DamageOverTimeData.h"

class Level;
class SpellManager;
class Spell;

// a MOB in a level, enemies + main character.
class LevelMovableGameObject : public MovableGameObject {
public:
	LevelMovableGameObject(Level* level);
	virtual ~LevelMovableGameObject();

	virtual void update(const sf::Time& frameTime) override;
	virtual void checkCollisions(const sf::Vector2f& nextPosition);
	// the offset to the from where a spell starts. it gets added to the spell offset defined by the spell itself. default is (0,0)
	virtual sf::Vector2f getConfiguredSpellOffset() const;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	virtual void onHit(Spell* spell);
	// adds damage to the attribute health. this damage can't be negative
	virtual void addDamage(int damage, DamageType damageType);
	// adds heal to the attribute health. the heal can't be negative
	void addHeal(int heal);
	// sets the dead bool and sets the attribute health to zero.
	virtual void setDead();
	// sets the fight animation time of this mob
	void setFightAnimationTime();
	// the mob consumes a food and gets its bonus attributes for its duration. they may also be negative.
	void consumeFood(const sf::Time& duration, const AttributeData& attributes);
	// the mob adds these attributes to its own. if their time runs out, they get removed again. The attribute "current health" however will stay.
	void addAttributes(const sf::Time& duration, const AttributeData& attributes);
	// adds a dot to this mob
	virtual void addDamageOverTime(const DamageOverTimeData& data);
	// gravity flip (used for anti gravity spell)
	void flipGravity();

	SpellManager* getSpellManager() const;
	const AttributeData* getAttributes() const;
	Level* getLevel() const;
	bool getIsFacingRight() const;
	bool getIsUpsideDown() const;
	bool isDead() const;
	GameObjectState getState() const;

protected:

	virtual float getConfiguredWalkAcceleration() const;
	virtual float getConfiguredGravityAcceleration() const;
	virtual sf::Time getConfiguredFightAnimationTime() const = 0;
	// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
	virtual float getConfiguredDampingGroundPersS() const;
	virtual float getConfiguredDampingAirPerS() const;
	// handle input and calculate the next position (AI or user input)
	virtual void handleMovementInput() = 0;
	virtual void handleAttackInput() = 0;
	// update animation based on the current velocity + grounded
	virtual void updateAnimation(const sf::Time& frameTime);
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	bool m_isGrounded = false;
	bool m_isDead = false;
	bool m_isFlippedGravity = false;
	Level* m_level;

	SpellManager* m_spellManager;
	// as long as this time is not sf::Time::Zero, the mob will have the fighting animation. 
	sf::Time m_fightAnimationTime = sf::Time::Zero;

	// store attributes given by food. if their time runs out, they get removed from the total attributes.
	std::pair<sf::Time, AttributeData> m_foodAttributes;
	// a vector to store the attributes given by spells (buffs). if their time runs out, they get removed from the total attributes.
	std::vector<std::pair<sf::Time, AttributeData>> m_buffAttributes;
	// active debuffs (dots) with their type, remaining time and dps
	std::vector<DamageOverTimeData> m_dots;

	AttributeData m_attributes;

	// attributes, include regeneration (hp) and all buffs & dots.
	void updateAttributes(const sf::Time& frameTime);
	sf::Time m_timeSinceRegeneration = sf::Time::Zero;
};