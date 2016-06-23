#include "ScreenOverlays/EnemyDefeatedScreenOverlay.h"
#include "ResourceManager.h"
#include "GUI/GUIConstants.h"

const float YOFFSET = 0.5f * WINDOW_HEIGHT;
const float COLUMN_MARGIN = 20.f;
const float TEXT_MARGIN = 10.f;

const float TEXT_WIDTH = 4 * InventorySlot::ICON_SIZE;
const float COLUMN_WIDTH = InventorySlot::SIZE + TEXT_MARGIN + TEXT_WIDTH;

EnemyDefeatedScreenOverlay::EnemyDefeatedScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) :
	ScreenOverlay(activeTime, fadeTime),
	TextureScreenOverlay(activeTime, fadeTime) {
	m_isPermanent = true;
	m_background.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_OVERLAY));

	const sf::Texture* text = g_resourceManager->getTexture(GlobalResource::TEX_TEXT_DEFEATED);
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
		m_items[i]->setAlpha((sf::Uint8)(m_scale * 255));
		m_texts[i]->setColor(sf::Color(255, 255, 255, (sf::Uint8)(m_scale * 255)));
	}
}

void EnemyDefeatedScreenOverlay::render(sf::RenderTarget& renderTarget) {
	TextureScreenOverlay::render(renderTarget);
	for (size_t i = 0; i < m_items.size(); ++i) {
		m_items[i]->render(renderTarget);
		renderTarget.draw(*m_texts[i]);
	}
}

void EnemyDefeatedScreenOverlay::setLoot(std::map<std::string, int>& items, int gold) {
	size_t nItems = items.size();
	if (gold > 0) nItems++;

	float width = nItems * COLUMN_WIDTH + (nItems - 1) * COLUMN_MARGIN;
	float xOffset = 0.5f * (WINDOW_WIDTH - width);

	int i = 0;
	for (auto& it : items) {
		InventorySlot* slot = new InventorySlot(it.first, it.second);
		slot->setPosition(sf::Vector2f(xOffset + InventorySlot::ICON_OFFSET, YOFFSET + InventorySlot::ICON_OFFSET));
		m_items.push_back(slot);

		std::string str = g_textProvider->getText(it.first, "item");
		if (it.second > 1) str += " x" + std::to_string(it.second);
		std::string croppedStr = g_textProvider->getCroppedString(str, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH));

		BitmapText* text = new BitmapText(croppedStr, TextStyle::Shadowed, TextAlignment::Left);
		text->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		sf::FloatRect bbox = text->getLocalBounds();
		text->setPosition(xOffset + InventorySlot::SIZE + TEXT_MARGIN,
				YOFFSET + InventorySlot::ICON_OFFSET + 0.5f * (InventorySlot::SIZE - 2 * InventorySlot::ICON_OFFSET - bbox.height));
		m_texts.push_back(text);

		xOffset += COLUMN_WIDTH + COLUMN_MARGIN;
		i++;
	}

	if (gold > 0) {
		InventorySlot* slot = new InventorySlot("gold", gold);
		slot->setPosition(sf::Vector2f(xOffset + InventorySlot::ICON_OFFSET, YOFFSET + InventorySlot::ICON_OFFSET));
		m_items.push_back(slot);

		std::string str = g_textProvider->getText("Gold");
		if (gold > 1) str += " x" + std::to_string(gold);
		std::string croppedStr = g_textProvider->getCroppedString(str, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH));

		BitmapText* text = new BitmapText(croppedStr, TextStyle::Shadowed, TextAlignment::Left);
		text->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		sf::FloatRect bbox = text->getLocalBounds();
		text->setPosition(xOffset + InventorySlot::SIZE + TEXT_MARGIN,
			YOFFSET + InventorySlot::ICON_OFFSET + 0.5f * (InventorySlot::SIZE - 2 * InventorySlot::ICON_OFFSET - bbox.height));
		m_texts.push_back(text);
	}
}