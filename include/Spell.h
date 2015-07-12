#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "Level.h"

#include "Enums/SpellID.h"
#include "Enums/DamageType.h"
#include "Structs/SpellBean.h"

class LevelMovableGameObject;

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:
	Spell() : MovableGameObject() {}
	// called by the spell manager
	void init(SpellBean& bean);
	// the spell divergence only matters if we have more than one spell. else, the spell divergence will be 0
	virtual void load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target, int divergence);
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
	int getDamage() const;
	virtual SpellID getConfiguredSpellID() const = 0;
	virtual DamageType getConfiguredDamageType() const = 0;
	GameObjectType getConfiguredType() const override;

	const sf::Time& getActiveTime() const;
	const MovableGameObject* getOwner() const;

private:
	Level* m_level;
	LevelMovableGameObject* m_mob; // owner, it will never hurt the owner or any other mob of the same type.
	GameObjectType m_ownerType;
	sf::Time m_activeCoolDown;

	int m_damage;
	int m_reflectCount;
	float m_speed;

	// enemy list from screen
	std::vector<GameObject*>* m_enemies; 
	// main character from screen
	LevelMainCharacter* m_mainChar;
	// calculates position according to m_mob
	void calculatePositionAccordingToMob(sf::Vector2f& position) const;
	// collisions with mainchar
	void checkCollisionsWithMainChar(const sf::FloatRect* boundingBox);
	// collisions with enemies
	void checkCollisionsWithEnemies(const sf::FloatRect* boundingBox);
	sf::Vector2f rotateVector(const sf::Vector2f &vec, float angle);
};