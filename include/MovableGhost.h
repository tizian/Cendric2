#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Structs/AIWalkingQueryRecord.h"
#include "Structs/WorldCollisionQueryRecord.h"
#include "Enums/LevelDynamicTileID.h"
#include "MovingGhostDebugger.h"

class Level;

// where and how did our ghost collide?
struct GhostRecord {
	CollisionDirection direction = CollisionDirection::VOID;
	float savePosY = -1.f;
	bool evilTile = false;
	bool collides = false;
};

// a ghost (dummy) of a movable game object, used to simulate its path.
class MovableGhost : public virtual MovableGameObject {
public:
	MovableGhost(const AIWalkingQueryRecord& rec, Level* level, Screen* screen);
	~MovableGhost();

	void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);

	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	float getConfiguredMaxVelocityX() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityYUp() const override;

	const GhostRecord& getGhostRecord() const;

	GameObjectType getConfiguredType() const override;

protected:
	Level* m_level;
	GhostRecord m_record;
	AIWalkingQueryRecord m_aiRec;
	MovingGhostDebugger* m_debugger = nullptr;

	// an ghost will set its "evil tile" flag if it collided with those. Default are spikes top and bottom.
	std::set<LevelDynamicTileID> m_avoidableTiles;
};