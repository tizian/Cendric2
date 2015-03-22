#pragma once

#include "stdafx.h"

// an abstract object with a gamestate and a bounding box
class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(sf::Time frameTime);
	virtual void render(sf::RenderTarget &renderTarget) const;

	void setScreen(Screen *screen);

private:
	Screen *m_screen;

	IntRect m_boundingBox;
};