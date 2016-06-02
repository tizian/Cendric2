#pragma once

#include "ScreenOverlays/TextureScreenOverlay.h"
#include "GUI/BitmapText.h"
#include "GUI/InventorySlot.h"

class EnemyDefeatedScreenOverlay : public virtual TextureScreenOverlay {
public:
	EnemyDefeatedScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	~EnemyDefeatedScreenOverlay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void setLoot(std::map<std::string, int>& items, int gold);

private:
	std::vector<InventorySlot*> m_items;
	std::vector<BitmapText*> m_texts;
};
