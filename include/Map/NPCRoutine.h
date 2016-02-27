#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Structs/DialogueNode.h"

class GameScreen;
class DialogueWindow;

enum class RoutineState {
	VOID,
	Waiting,
	GoingTo
};

struct RoutineStep {
	RoutineState state;
	sf::Time time;
	sf::Vector2f goal;
};

class NPC;

// A routine of a npcs behavior
class NPCRoutine {
public:
	void load(const std::string& id, NPC* npc);
	const std::string& getID() const;
	void update(const sf::Time& frameTime);

	// called from the loader
	void addStep(const RoutineStep& step);
	void setLooping(bool looping);
	void setVelocity(float velocity);
	NPC* getNPC() const;

private:
	NPC* m_npc;
	std::string m_id = "";
	bool m_isLooping = true;
	std::vector<RoutineStep> m_steps;
	sf::Time m_remainingStepTime = sf::Time::Zero;
	int m_currentStepID;
	float m_velocity = 50.f;
};