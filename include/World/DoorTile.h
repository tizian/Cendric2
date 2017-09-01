#pragma once

#include "global.h"
#include "Structs/Condition.h"
#include "World/AnimatedGameObject.h"

class InteractComponent;
class MainCharacter;

class DoorTile : public virtual AnimatedGameObject {
public:
	virtual ~DoorTile() {}

	void notifyReloadNeeded();

protected:
	virtual void open() = 0;
	virtual void close() = 0;
	virtual float getOpenRange() const = 0;

protected:
	void reloadConditions(MainCharacter* mainChar);
	void initConditions(const std::map<std::string, std::string>& properties);

protected:
	InteractComponent* m_interactComponent;
	bool m_isOpen;
	bool m_isInitialized = false;
	bool m_isReloadNeeded = false;
	bool m_isConditionsFulfilled;
	int m_strength = 0;
	
	std::string m_keyItemID;
	std::vector<Condition> m_conditions;
};