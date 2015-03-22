#include "stdafx.h"

GameStateController::GameStateController(GameState *initialState)
{
	m_currentState = initialState;
	m_currentState->setGameStateController(this);
}

void GameStateController::update(float dt)
{
	m_currentState->update(dt);
}

void GameStateController::render(sf::RenderTarget &renderTarget) const
{
	m_currentState->render(renderTarget);
}

GameState *GameStateController::getCurrentState() const
{
	return m_currentState;
}

void GameStateController::changeStateTo(GameState *nextState)
{
	GameState *previousState = m_currentState;
	previousState->onExit(nextState);
	m_currentState = nextState;
	m_currentState->setGameStateController(this);
	m_currentState->onEnter(previousState);
	// delete previousState;	// TODO(tizian): Memory leak?
}