#include "GameObjectComponents/InteractComponent.h"
#include "World/MainCharacter.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

InteractComponent::InteractComponent(std::string tooltip, AnimatedGameObject* parent, MainCharacter* mainChar) : 
	GameObjectComponent(parent), 
	TooltipComponent(tooltip, parent, true) {
	
	m_mainChar = mainChar;
	m_isInteractable = true;
	m_isFocused = false;
	m_interactRange = 50.f;

	m_interactText.setTextStyle(TextStyle::Shadowed);
	m_interactText.setColor(COLOR_GOOD);
	m_interactText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);

	setInteractText("ToInteract");

	setShowOnTooltipToggle(true);
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

bool InteractComponent::isInteractable() const {
	return m_isInteractable;
}

void InteractComponent::setOnInteract(const std::function<void()>& agent) {
	m_executeOnInteract = agent;
}

void InteractComponent::interact() {
	m_executeOnInteract();
}

void InteractComponent::setInteractText(const std::string& textKey) {
	std::string interactString = "<";
	interactString.append(EnumNames::getShortKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Interact)));
	interactString.append("> " + g_textProvider->getText(textKey));
	m_interactText.setString(interactString);
}

void InteractComponent::setInteractTextColor(const sf::Color& color) {
	m_interactText.setColor(color);
}

void InteractComponent::setInteractRange(float range) {
	m_interactRange = range;
}

void InteractComponent::setInteractable(bool interactable) {
	m_isInteractable = interactable;
	m_useInteractiveColor = interactable;
}

void InteractComponent::setFocused(bool focused) {
	m_isFocused = focused;
	setPosition(m_animatedParent->getPosition());
}

void InteractComponent::onDisposed() {
	m_mainChar->notifyDisposed(this);
}

void InteractComponent::setPosition(const sf::Vector2f& pos) {
	if (!m_isFocused) {
		TooltipComponent::setPosition(pos);
	} else {
		m_tooltipText.setPosition(pos + sf::Vector2f(
			0.5f * (m_parent->getBoundingBox()->width - m_tooltipText.getLocalBounds().width),
			-m_tooltipHeight - GUIConstants::CHARACTER_SIZE_M));
		m_interactText.setPosition(pos + sf::Vector2f(
			0.5f * (m_parent->getBoundingBox()->width - m_interactText.getLocalBounds().width),
			-m_tooltipHeight));
	}
}

void InteractComponent::renderAfterForeground(sf::RenderTarget& renderTarget) {
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (m_isFocused || showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
	if (m_isFocused) {
		renderTarget.draw(m_interactText);
	}
}
