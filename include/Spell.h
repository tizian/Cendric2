#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "Level/Level.h"

#include "Enums/SpellID.h"
#include "Enums/DamageType.h"
#include "Structs/SpellData.h"

class LevelMovableGameObject;

// A spell cendric can cast
class Spell : virtual public MovableGameObject {
public:
	Spell() : MovableGameObject() {}
	virtual ~Spell() {}

	virtual void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target);
	virtual void update(const sf::Time& frameTime) override;
	virtual void setViewable(bool value) override;

	virtual void checkCollisions(const sf::Vector2f& nextPosition);

	// the offset of the spells start position, as seen from the upper mid of the mobs bounding box. The default is the position of the staff head
	virtual sf::Vector2f getConfiguredPositionOffset() const;
	// if true, the spell sprite will be rotated accordingly. default is true.
	virtual bool getConfiguredRotateSprite() const;
	int getDamage() const;
	int getDamagePerSecond() const;
	int getHeal() const;
	SpellID getSpellID() const;
	SpellType getSpellType() const;
	DamageType getDamageType() const;
	GameObjectType getConfiguredType() const override;

	// effects executed on mob when it hits one. default does remove the spell object. executed by the mob itself.
	virtual void execOnHit(LevelMovableGameObject* target);

	const sf::Time& getActiveDuration() const;
	const sf::Time& getDuration() const;
	const MovableGameObject* getOwner() const;
	bool isAttachedToMob() const;

protected:
	SpellData m_data;
	
	Level* m_level;
	LevelMovableGameObject* m_mob; // owner, it will never hurt the owner or any other mob of the same type.
	GameObjectType m_ownerType;
	
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
	sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle);
};