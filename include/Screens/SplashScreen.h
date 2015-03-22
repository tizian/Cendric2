#include "stdafx.h"

class SplashScreen : public Screen
{
public:
	SplashScreen();

	Screen* update(float dt) override;
	void render(sf::RenderTarget &renderTarget) const override; 

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

private:
	sf::Texture m_screenTexture;	// TODO: take from resource manager
	sf::Sprite m_screenSprite;
};