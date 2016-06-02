#pragma once

#include "ScreenOverlays/TextureScreenOverlay.h"
#include "GUI/BitmapText.h"
#include "GUI/InventorySlot.h"

class EnemyDefeatedScreenOverlay : public virtual TextureScreenOverlay {
public:
	EnemyDefeatedScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void setLootItems(std::map<std::string, int>& items);
	void setGold(int gold);

private:
	std::vector<InventorySlot> m_sprites;
	std::vector<BitmapText> m_texts;
};
