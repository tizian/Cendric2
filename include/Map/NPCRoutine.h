#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Structs/DialogueNode.h"
#include "Structs/RoutineStep.h"

class GameScreen;
class DialogueWindow;
class NPC;

// A routine of a npcs behavior
class NPCRoutine final {
public:
	void load(const std::string& id, NPC* npc, bool initial = true);
	const std::string& getID() const;
	void update(const sf::Time& frameTime);

	// called from the loader
	void addStep(const RoutineStep& step);
	void setLooped(bool looped);
	void setVelocity(float velocity);
	NPC* getNPC() const;

private:
	NPC* m_npc;
	std::string m_id = "";
	bool m_isLooped = true;
	std::vector<RoutineStep> m_steps;
	sf::Time m_remainingStepTime = sf::Time::Zero;
	int m_currentStepID;
	float m_velocity = 50.f;
};