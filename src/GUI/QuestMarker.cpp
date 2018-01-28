#include "GUI/QuestMarker.h"
#include "ResourceManager.h"
#include "GlobalResource.h"
#include "CharacterCore.h"
#include "GameObjectComponents/TooltipWindowComponent.h"

const int QuestMarker::SIZE = 20;

QuestMarker::QuestMarker(const QuestData& questData, CharacterCore* core) {
	m_questData = questData;
	m_characterCore = core;
	m_isInputInDefaultView = true;
	
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(SIZE), static_cast<float>(SIZE)));
	m_sprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_QUESTMARKERS));

	m_tooltipComponent = new TooltipWindowComponent("", this);
	m_tooltipComponent->setWindowOffset(sf::Vector2f(0.f, SIZE + 4.f));
	addComponent(m_tooltipComponent);
}

void QuestMarker::render(sf::RenderTarget& renderTarget) {
	GameObject::render(renderTarget);
	renderTarget.draw(m_sprite);
}

void QuestMarker::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_sprite.setPosition(pos);
}

bool QuestMarker::isActive() const {
	return m_isActive;
}

void QuestMarker::setActive(bool active) {
	m_isActive = active;
	updateQuestMarkerState();
	execSetActive();
	auto const y = m_questData.isMainQuest ? 1 : 0;
	auto const x = static_cast<int>(m_markerState);
	m_sprite.setTextureRect(sf::IntRect(x * SIZE, y * SIZE, SIZE, SIZE));
}

std::vector<QuestMarkerData> QuestMarker::getCurrentStepData(const QuestData& questData, CharacterCore* core)
{
	std::vector<QuestMarkerData> stepData;
	int currentStep = 0;
	if (core->isQuestComplete(questData.id)) {
		currentStep = -1;
	}
	else {
		// find the current step
		while (true) {
			if (!core->isQuestDescriptionUnlocked(questData.id, currentStep + 1)) {
				break;
			}
			currentStep++;
		}
	}
	
	for (auto& data : questData.questMarkers)
	{
		if (data.step == currentStep && (data.mapId.empty() || core->isMapExplored(data.mapId)))
		{
			stepData.push_back(data);
		}
	}
	return stepData;
}

void QuestMarker::updateQuestMarkerState() {
	if (!m_isActive) {
		m_markerState = QuestMarkerState::Inactive;
		return;
	}
	if (m_characterCore->isQuestComplete(m_questData.id)) {
		m_markerState = QuestMarkerState::Completed;
	}
	else {
		m_markerState = QuestMarkerState::InProgress;
	}
}