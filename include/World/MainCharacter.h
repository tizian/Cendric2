#pragma once

#include "global.h"
#include "World/GameObject.h"

class InteractComponent;

// Cendric on a map or in a level.
class MainCharacter : public virtual GameObject {
public:
	MainCharacter() {};
	virtual ~MainCharacter() {};

	// registers an interactive object that's in range
	void registerInteractiveObject(InteractComponent* component);
	// notifies that an interactive object has been disposed
	void notifyDisposed(InteractComponent* component);

protected:
	void handleInteraction();

protected:
	std::vector<InteractComponent*> m_interactiveObjects;
	InteractComponent* m_nearestInteractive = nullptr;
};