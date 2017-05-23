#include "World/BackgroundLayer.h"

using namespace std;

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}

void BackgroundLayer::dispose() {
	g_resourceManager->deleteUniqueResources(this);
}

void BackgroundLayer::load(string& filename, float distance) {
	m_fileName = filename;
	m_distance = distance;
	g_resourceManager->loadTexture(m_fileName, ResourceType::Unique, this);
	const sf::Texture* tex = g_resourceManager->getTexture(m_fileName);
	if (tex) {
		m_sprite = sf::Sprite(*tex);
	}
}

void BackgroundLayer::render(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_sprite, states);
}

float BackgroundLayer::getDistance() const {
	return m_distance;
}