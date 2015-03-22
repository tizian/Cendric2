#pragma once

#include "stdafx.h"

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget &renderTarget) const;

	void setScreen(Screen *screen);

private:
	Screen *m_screen;

	IntRect m_boundingBox;
};