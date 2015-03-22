#include "stdafx.h"

class MenuScreen : public Screen
{
public:
	MenuScreen();

	void update(float dt) override;
	void render(sf::RenderTarget &renderTarget) const override;

private:
	sf::Texture m_screenTexture;	// TODO: take from resource manager
	sf::Sprite m_screenSprite;
};