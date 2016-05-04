#include "MainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"

void MainCharacter::handleInteraction() {
	if (m_nearestInteractive == nullptr && m_interactiveObjects.empty()) return;
	if (m_nearestInteractive != nullptr) {
		m_nearestInteractive->setFocused(false);
		m_nearestInteractive = nullptr;
	}

	float nearest = 100000;

	for (auto& obj : m_interactiveObjects) {
		float newNearest = obj->getDistanceToMainChar();
		if (obj->getDistanceToMainChar() < nearest) {
			nearest = newNearest;
			m_nearestInteractive = obj;
		}
	}

	if (m_nearestInteractive != nullptr) {
		m_nearestInteractive->setFocused(true);
		if (g_inputController->isKeyJustPressed(Key::Interact)) {
			m_nearestInteractive->interact();
			m_nearestInteractive->setFocused(false);
			m_nearestInteractive = nullptr;
		}
	}

	m_interactiveObjects.clear();
}

void MainCharacter::registerInteractiveObject(InteractComponent* component) {
	m_interactiveObjects.push_back(component);
}
