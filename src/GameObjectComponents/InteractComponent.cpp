#include "GameObjectComponents/InteractComponent.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

InteractComponent::InteractComponent(std::string tooltip, AnimatedGameObject* parent, MainCharacter* mainChar) : 
	GameObjectComponent(parent), 
	TooltipComponent(tooltip, parent, true) {
	
	m_mainChar = mainChar;
	m_isFocused = false;
	m_interactRange = 50.f;
	m_tooltipString = tooltip;
	setInteractText("ToInteract");

	m_executeOnInteract = std::bind(&InteractComponent::nop, this);
}

void InteractComponent::update(const sf::Time& frameTime) {
	TooltipComponent::update(frameTime);

	if (!m_isInteractable || m_animatedParent->isDisposed()) return;

	if (getDistanceToMainChar() <= m_interactRange) {
		m_mainChar->registerInteractiveObject(this);
	}
}

float InteractComponent::getDistanceToMainChar() const {
	return dist(m_mainChar->getCenter(), m_animatedParent->getCenter());
}

void InteractComponent::setOnInteract(const std::function<void()>& agent) {
	m_executeOnInteract = agent;
}

void InteractComponent::interact() {
	m_executeOnInteract();
}

void InteractComponent::setInteractText(const std::string& textKey) {
	m_interactString = g_textProvider->getText("Press", "hint_desc") + " ";
	m_interactString.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Interact)));
	m_interactString.append(" " + g_textProvider->getText(textKey));
}

void InteractComponent::setInteractRange(float range) {
	m_interactRange = range;
}

void InteractComponent::setInteractable(bool interactable) {
	m_isInteractable = interactable;
}

void InteractComponent::setFocused(bool focused) {
	m_isFocused = focused;
	if (m_isFocused) {
		m_tooltipText.setString(m_interactString);
		m_tooltipText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	} else {
		m_tooltipText.setString(m_tooltipString);
		m_tooltipText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}
	setPosition(m_animatedParent->getPosition());
}

void InteractComponent::renderAfterForeground(sf::RenderTarget& renderTarget) {
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (m_isFocused || showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}
