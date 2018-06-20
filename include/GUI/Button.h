#pragma once

#include "global.h"
#include "GUI/ButtonInterface.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"
#include "GUI/OrnamentStyle.h"

// A not so ugly button to click
class Button final : public ButtonInterface {
public:
	Button(const sf::FloatRect& box, GUIOrnamentStyle style = GUIOrnamentStyle::NONE);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;

	void setPosition(const sf::Vector2f& pos) override;
	void setSize(const sf::Vector2f& size) override;

	void setOnClick(const std::function<void()>& agent);
	void click() override;

	void setGamepadKey(Key key);

	// position will be set automatically as the center of the button.
	// setting text using the text provider (translated)
	void setText(const std::string& text, const sf::Color& color, int charSize);
	void setText(const std::string& text, int charSize);
	void setText(const std::string& text);

	// setting raw text without text provider (not translated)
	void setTextRaw(const std::string& text, const sf::Color& color, int charSize);
	void setTextRaw(const std::string& text, int charSize);
	void setTextRaw(const std::string& text);

	void setCharacterSize(int size);
	void setTextColor(const sf::Color& color);

	// texture
	void setTexture(const sf::Texture* texture);
	void setTextureColor(const sf::Color& color);

	void setOrnamentLayerTexture(sf::Texture* texture);

	// a button can only be clicked if its enabled. also, the color is less opaque if it is disabled.
	void setEnabled(bool enabled) override;

	void setBackgroundLayerColor(const sf::Color& color);
	void setMainLayerColor(const sf::Color& color);
	void setOrnamentLayerColor(const sf::Color& color);
	void setMouseOverColor(const sf::Color& color);
	void setSelectedColor(const sf::Color& color);
	
	sf::Color getBackgroundLayerColor() const;
	sf::Color getMainLayerColor() const;
	sf::Color getOrnamentLayerColor() const;
	sf::Color getMouseOverColor() const;

protected:
	int m_characterSize;
	bool m_hasTexture;

	sf::Vector2f m_positionDefault;
	sf::Vector2f m_backLayerOffset;
	sf::Vector2f m_textOffset;
	sf::Vector2f m_keyTextOffset;
	sf::Vector2f m_textureOffset;

	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;
	sf::Sprite m_textureLayer;

	sf::Color m_backLayerColor;
	sf::Color m_mainLayerColor;
	sf::Color m_ornamentLayerColor;
	sf::Color m_mouseOverColor;
	sf::Color m_selectedColor;
	sf::Color m_textureLayerColor;

	BitmapText m_text;
	BitmapText m_keyText;

	void updateColor() override;
	void reloadTextPosition();
	float checkCharacterSize(int charSize);
	bool hasGamepadKey() const;

	Key m_gamepadKey = Key::VOID;

private:
	std::function<void()> m_executeOnClick = [](){};
};