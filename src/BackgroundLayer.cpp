#include "BackgroundLayer.h"

using namespace std;

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}

void BackgroundLayer::dispose() {
	g_resourceManager->deleteResource(m_fileName);
}

void BackgroundLayer::load(string& filename, float distance) {
	m_fileName = filename;
	m_distance = distance;
	m_sprite = sf::Sprite((*g_resourceManager->getTexture(m_fileName)));
}

void BackgroundLayer::render(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_sprite, states);
}

float BackgroundLayer::getDistance() const {
	return m_distance;
}