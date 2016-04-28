#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Structs/AIWalkingQueryRecord.h"
#include "Structs/WorldCollisionQueryRecord.h"
#include "Enums/LevelDynamicTileID.h"
#include "Level/MOBBehavior/JumpingGhostDebugger.h"
#include "ResourceManager.h"

class Level;

// where and how did our ghost collide?
struct GhostRecord {
	CollisionDirection direction = CollisionDirection::VOID;
	float savePosY = -1.f;
	bool evilTile = false;
	bool collides = false;
};

// a ghost (dummy) of a movable game object, used to simulate its path.
class JumpingGhost : public virtual MovableGameObject {
public:
	JumpingGhost(const AIWalkingQueryRecord& rec, const Level* level, Screen* screen);
	~JumpingGhost();

	void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);

	// calculates a jump from the given AI jump record
	// returns a feasible y position if the jump succeeds
	// or -1.f if it won't.
	float calculateJump();

	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	float getConfiguredMaxVelocityX() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityYUp() const override;

	const GhostRecord& getGhostRecord() const;

	GameObjectType getConfiguredType() const override;

protected:
	const Level* m_level;
	GhostRecord m_record;
	AIWalkingQueryRecord m_aiRec;
	JumpingGhostDebugger* m_debugger = nullptr;
};