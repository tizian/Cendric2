#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "Level/Level.h"

#include "Enums/SpellID.h"
#include "Enums/DamageType.h"
#include "Structs/SpellData.h"

#include "Particles/ParticleGenerator.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleUpdater.h"

#include "LightObject.h"

class LevelMovableGameObject;
class LevelDynamicTile;

// A spell cendric can cast
class Spell : public virtual MovableGameObject {
public:
	Spell() : MovableGameObject() {}
	virtual ~Spell() {}

	virtual void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target);
	// there are spells that need a mob and can't use a tile as owner. If the spell is attached to a mob, this 
	// method throws an exception.
	virtual void load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target);
	virtual void update(const sf::Time& frameTime) override;
	virtual void setViewable(bool value) override;

	virtual void checkCollisions(const sf::Vector2f& nextPosition);

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
	// gets called when the owner is killed(disposed)
	virtual void onOwnerDisposed();

	const sf::Time& getActiveDuration() const;
	const sf::Time& getDuration() const;
	const MovableGameObject* getOwner() const;
	bool isAttachedToMob() const;

protected:
	SpellData m_data;
	
	const Level* m_level;
	LevelMovableGameObject* m_mob; // owner, it will never hurt the owner or any other mob of the same type.
	
	// enemy list from screen
	std::vector<GameObject*>* m_enemies;
	// main character from screen
	LevelMainCharacter* m_mainChar;
	// calculates position according to m_mob
	void calculatePositionAccordingToMob(sf::Vector2f& position) const;
	// collisions with mainchar and allied enemies
	void checkCollisionsWithAllies(const sf::FloatRect* boundingBox);
	// collisions with enemies
	virtual void checkCollisionsWithEnemies(const sf::FloatRect* boundingBox);
	sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle);
};