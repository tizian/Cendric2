#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Structs/AttributeBean.h"
#include "Enums/DebuffType.h"

class Level;
class SpellManager;
class Spell;

// a MOB in a level, enemies + main character.
class LevelMovableGameObject : public MovableGameObject
{
public:
	LevelMovableGameObject(Level* level);
	~LevelMovableGameObject();

	virtual void update(const sf::Time& frameTime) override;
	virtual void checkCollisions(const sf::Vector2f& nextPosition) override;
	// the offset to the from where a spell starts. it gets added to the spell offset defined by the spell itself. default is (0,0)
	virtual sf::Vector2f getConfiguredSpellOffset() const;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	virtual void onHit(Spell* spell);
	// adds damage to the attribute health. this damage can't be negative
	void addDamage(int damage);
	// adds heal to the attribute health. the heal can't be negative
	void addHeal(int heal);
	// sets the dead bool and sets the attribute health to zero.
	void setDead();
	// sets the fight animation time of this mob
	void setFightAnimationTime();
	// the mob consumes a food and gets its bonus attributes for its duration. they may also be negative.
	void consumeFood(sf::Time& duration, AttributeBean& attributes);
	// the mob adds these attributes to its own. if their time runs out, they get removed again. The attribute "current health" however will stay.
	void addAttributes(sf::Time& duration, AttributeBean& attributes);

	SpellManager* getSpellManager() const;
	const AttributeBean* getAttributes() const;
	Level* getLevel() const;
	bool getIsFacingRight() const;
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
	virtual void setSpriteColor(const sf::Color& color, const sf::Time& time);
	// update animation based on the current velocity + grounded
	void updateAnimation();
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	bool m_isGrounded = false;
	bool m_isDead = false;
	Level* m_level;

	SpellManager* m_spellManager;
	// as long as this time is not sf::Time::Zero, the mob will have the fighting animation. 
	sf::Time m_fightAnimationTime = sf::Time::Zero;
	// the sprite will reset its color as soon as this time is zero.
	sf::Time m_coloredTime = sf::Time::Zero;

	// store attributes given by food. if their time runs out, they get removed from the total attributes.
	std::pair<sf::Time, AttributeBean> m_foodAttributes;
	// a vector to store the attributes given by spells (buffs). if their time runs out, they get removed from the total attributes.
	std::vector<std::pair<sf::Time, AttributeBean>> m_buffAttributes;

	AttributeBean m_attributes;

	// attributes, include regeneration (hp) and all buffs.
	void updateAttributes(const sf::Time& frameTime);
	sf::Time m_timeSinceRegeneration = sf::Time::Zero;

	// current debuffs
	void updateDebuffs(const sf::Time& frameTime);
	std::map<DebuffType, sf::Time> m_currentDebuffs;
};