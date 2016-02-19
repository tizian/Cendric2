#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Structs/AttributeData.h"
#include "Structs/DamageOverTimeData.h"

class Level;
class SpellManager;
class Spell;
class MovingBehavior;
class AttackingBehavior;

// a MOB in a level, enemies + main character.
class LevelMovableGameObject : public virtual MovableGameObject {
public:
	LevelMovableGameObject(Level* level);
	virtual ~LevelMovableGameObject();

	virtual void update(const sf::Time& frameTime) override;

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
	virtual void addDamageOverTime(DamageOverTimeData& data);
	// the mob flees for the given time
	virtual void setFeared(const sf::Time& fearedTime);
	// the mob is stunned for the given time
	virtual void setStunned(const sf::Time& stunnedTime);
	// flips the gravity and the sprite
	void flipGravity();
	
	SpellManager* getSpellManager() const;
	AttackingBehavior* getAttackingBehavior() const;
	MovingBehavior* getMovingBehavior() const;
	const AttributeData* getAttributes() const;
	Level* getLevel() const;
	bool isFacingRight() const;
	bool isUpsideDown() const;
	bool isIgnoreDynamicTiles() const;
	bool isDead() const;
	GameObjectState getState() const;

	float getConfiguredMaxVelocityX() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityYUp() const override;

protected:
	// loads the behavior, deleting potential old ones.
	virtual void loadBehavior();

	// behavior strategies aka "components"
	virtual MovingBehavior* createMovingBehavior(bool asAlly = false) = 0;
	virtual AttackingBehavior* createAttackingBehavior(bool asAlly = false) = 0;

	MovingBehavior* m_movingBehavior = nullptr;
	AttackingBehavior* m_attackingBehavior = nullptr;

	bool m_isDead = false;
	bool m_isImmortal = false;
	Level* m_level;

	SpellManager* m_spellManager;

	// store attributes given by food. if their time runs out, they get removed from the total attributes.
	std::pair<sf::Time, AttributeData> m_foodAttributes;
	// a vector to store the attributes given by spells (buffs). if their time runs out, they get removed from the total attributes.
	std::vector<std::pair<sf::Time, AttributeData>> m_buffAttributes;
	// active debuffs (dots) with their type, remaining time and dps
	std::vector<DamageOverTimeData> m_dots;
	// time stunned
	sf::Time m_stunnedTime = sf::Time::Zero;
	// time feared
	sf::Time m_fearedTime = sf::Time::Zero;

	AttributeData m_attributes;

	// attributes, include regeneration (hp) and all buffs & dots.
	void updateAttributes(const sf::Time& frameTime);
	sf::Time m_timeSinceRegeneration = sf::Time::Zero;
};