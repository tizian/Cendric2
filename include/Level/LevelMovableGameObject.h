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
class DamageNumbers;

// a MOB in a level, enemies + main character.
class LevelMovableGameObject : public virtual MovableGameObject {
public:
	LevelMovableGameObject(const Level* level);
	virtual ~LevelMovableGameObject();

	virtual void update(const sf::Time& frameTime) override;

	void renderAfterForeground(sf::RenderTarget& target) override;
	
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	virtual void onHit(Spell* spell);
	// adds damage to the attribute health. this damage can't be negative
	virtual void addDamage(int damage, DamageType damageType, bool overTime, bool critical);
	// adds heal to the attribute health. the heal can't be negative
	void addHeal(int heal, bool overTime, bool critical);
	// sets the dead bool and sets the attribute health to zero.
	virtual void setDead();
	// executes the fight animation of this mob (using the default fight animation) and returns the time of that animation
	sf::Time executeDefaultFightAnimation(bool isBlocking, int times = 1);
	// executes the fight animation of this mob 
	void executeFightAnimation(const sf::Time& fightAnimationTime, GameObjectState animation, bool isBlocking);
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
	// cancels all blocking spells and fight animations and sets the state to idle
	void setReady();
	// sets the mob invincible
	void setInvincible(bool value);
	// flips the gravity and the sprite
	void flipGravity();
	// forces a facing right value
	void setFacingRight(bool value);
	// clears spells owned by this mob
	// argument true clears all spells
	// argument false clears only the attached spells, as if the enemy died.
	void clearSpells(bool clearAll);
	
	SpellManager* getSpellManager() const;
	AttackingBehavior* getAttackingBehavior() const;
	MovingBehavior* getMovingBehavior() const;
	const AttributeData* getAttributes() const;
	int getActiveSpellCount() const;
	const Level* getLevel() const;
	bool isFacingRight() const;
	bool isUpsideDown() const;
	bool isIgnoreDynamicTiles() const;
	bool isDead() const;
	bool isImmortal() const;
	bool isStunned() const;
	bool isFeared() const;
	bool isReady() const;
	bool isEating() const;
	GameObjectState getState() const;

	virtual bool isAlly() const;

	float getConfiguredMaxVelocityX() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityYUp() const override;

protected:
	// loads resources (textures, sound) for this mob
	virtual void loadResources();
	// loads the behavior, deleting potential old ones.
	virtual void loadBehavior();

	// behavior strategies aka "components"
	virtual MovingBehavior* createMovingBehavior(bool asAlly = false) = 0;
	virtual AttackingBehavior* createAttackingBehavior(bool asAlly = false) = 0;

	MovingBehavior* m_movingBehavior = nullptr;
	AttackingBehavior* m_attackingBehavior = nullptr;

	bool m_isDead = false;
	bool m_isImmortal = false; // this mob can't die (but takes damage)
	bool m_isInvincible = false; // this mob won't take any damage
	const Level* m_level = nullptr;

	SpellManager* m_spellManager = nullptr;

	bool m_displayDamageNumbers;
	DamageNumbers* m_damageNumbers = nullptr;

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

	// the sprite path of this mob, used for resource loading
	// can be empty if the mob has no sprite.
	virtual std::string getSpritePath() const { return ""; }
	virtual std::string getDeathSoundPath() const { return ""; }

	sf::Sound m_deathSound;
};