#include "ScreenOverlays/EnemyDefeatedScreenOverlay.h"

EnemyDefeatedScreenOverlay::EnemyDefeatedScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) :
	TextureScreenOverlay(activeTime, fadeTime),
	ScreenOverlay(activeTime, fadeTime) {
	//
}

void EnemyDefeatedScreenOverlay::update(const sf::Time& frameTime) {
	TextureScreenOverlay::update(frameTime);
	//
}

void EnemyDefeatedScreenOverlay::render(sf::RenderTarget& renderTarget) {
	//
	TextureScreenOverlay::render(renderTarget);
}

void EnemyDefeatedScreenOverlay::setLootItems(std::map<std::string, int>& items) {

}

void EnemyDefeatedScreenOverlay::setGold(int gold) {

}