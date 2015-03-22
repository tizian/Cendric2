#pragma once

#include "stdafx.h"

class GameStateController
{
public:
	GameStateController() = delete;
	GameStateController(GameState *initialState);

	void update(float dt);
	void render(sf::RenderTarget &renderTarget) const;

	GameState *getCurrentState() const;

	void changeStateTo(GameState *nextState);

private:
	GameState *m_currentState;
};