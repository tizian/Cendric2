#pragma once

#include "stdafx.h"

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget &renderTarget) const;

	void setGameState(GameState *gameState);

private:
	GameState *m_gameState;

	IntRect m_boundingBox;
};