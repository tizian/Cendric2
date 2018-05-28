#pragma once

#include "GUI/TabBar.h"
#include "GUI/TexturedTabButton.h"

class TexturedTabBar final : public TabBar {
public:
	TexturedTabBar() = default;
	~TexturedTabBar() {};

	void init(const sf::FloatRect& box, int numberTabs) override;

	TexturedTabButton* getTabButton(int index) const override;
};