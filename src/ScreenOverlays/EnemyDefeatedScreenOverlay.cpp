#include "ScreenOverlays/EnemyDefeatedScreenOverlay.h"
#include "ResourceManager.h"

const float MARGIN = 40.f;
const float yOffset = 0.66f * WINDOW_HEIGHT + InventorySlot::ICON_OFFSET;

EnemyDefeatedScreenOverlay::EnemyDefeatedScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) :
	TextureScreenOverlay(activeTime, fadeTime),
	ScreenOverlay(activeTime, fadeTime) {
	m_isPermanent = true;
	m_background.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_screen_overlay));

	const sf::Texture* text = g_resourceManager->getTexture(ResourceID::Texture_text_defeated);
	m_sprite.setTexture(*text);
	m_sprite.setScale(sf::Vector2f(3.f, 3.f));
	m_sprite.setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 3.f * text->getSize().x), 200.f - 0.5f * 3.f * 60.f));

	Language language = g_resourceManager->getConfiguration().language;
	if (language == Language::Lang_EN) {
		m_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(text->getSize().x), 60));
	}
	else if (language == Language::Lang_DE) {
		m_sprite.setTextureRect(sf::IntRect(0, 60, static_cast<int>(text->getSize().x), 60));
	}
	else if (language == Language::Lang_CH) {
		m_sprite.setTextureRect(sf::IntRect(0, 120, static_cast<int>(text->getSize().x), 60));
	}
}

EnemyDefeatedScreenOverlay::~EnemyDefeatedScreenOverlay() {
	for (size_t i = 0; i < m_items.size(); ++i) {
		delete m_items[i];
	}
	for (size_t i = 0; i < m_texts.size(); ++i) {
		delete m_texts[i];
	}
}

void EnemyDefeatedScreenOverlay::update(const sf::Time& frameTime) {
	TextureScreenOverlay::update(frameTime);
	for (size_t i = 0; i < m_items.size(); ++i) {
		InventorySlot* slot = m_items[i];
		slot->setAlpha((sf::Uint8)(m_scale * 255));
	}
}

void EnemyDefeatedScreenOverlay::render(sf::RenderTarget& renderTarget) {
	TextureScreenOverlay::render(renderTarget);
	for (size_t i = 0; i < m_items.size(); ++i) {
		InventorySlot* slot = m_items[i];
		slot->render(renderTarget);
	}
}

void EnemyDefeatedScreenOverlay::setLoot(std::map<std::string, int>& items, int gold) {
	size_t nItems = items.size();

	float width = nItems * InventorySlot::SIZE + (nItems - 1) * MARGIN;
	float xOffset = 0.5f * (WINDOW_WIDTH - width) + InventorySlot::ICON_OFFSET;

	int i = 0;
	for (auto& it : items) {
		InventorySlot* slot = new InventorySlot(it.first, it.second);

		slot->setPosition(sf::Vector2f(xOffset, yOffset));

		m_items.push_back(slot);

		xOffset += MARGIN + InventorySlot::SIZE;
		i++;
	}
}