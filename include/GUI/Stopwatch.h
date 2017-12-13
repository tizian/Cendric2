#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include "GUI/BitmapText.h"

class Stopwatch final : public GameObject {
public:
	Stopwatch();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	
	void restart();
	
	GameObjectType getConfiguredType() const override;

protected:
	BitmapText m_text;
	sf::Time m_time;
	sf::Clock m_stopwatch;
};