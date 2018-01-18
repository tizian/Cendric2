#include "GUI/QuestMarker.h"
#include "ResourceManager.h"
#include "GlobalResource.h"
#include "CharacterCore.h"

const float QuestMarker::SIZE = 16.f;

QuestMarker::QuestMarker(const QuestData& questData) {
	m_questData = questData;
	
	setBoundingBox(sf::FloatRect(0.f, 0.f, SIZE, SIZE));
	m_sprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_QUESTMARKERS));
}

void QuestMarker::render(sf::RenderTarget& renderTarget) {
	GameObject::render(renderTarget);
	renderTarget.draw(m_sprite);
}

void QuestMarker::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_sprite.setPosition(pos);
}

void QuestMarker::setActive(bool active) {
	int x = m_questData.isMainQuest ? SIZE : 0;
	m_sprite.setTextureRect(sf::IntRect(x, 0, SIZE, SIZE));
}