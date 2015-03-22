#pragma once

#include "stdafx.h"

class ScreenManager
{
public:
	ScreenManager();
	ScreenManager(Screen *initialScreen);

	void update(float dt);
	void render(sf::RenderTarget &renderTarget) const;

	Screen *getCurrentScreen();

	void changeScreenTo(Screen *nextScreen);

private:
	Screen *m_currentScreen = nullptr;
	Screen *m_nextScreen = nullptr;
};