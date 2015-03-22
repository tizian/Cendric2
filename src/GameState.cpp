#include "stdafx.h"

GameState::GameState()
{
	m_objects = vector<Object *>();
}

GameState::~GameState() {}

void GameState::update(float dt)
{
	for (Object *obj : m_objects)
	{
		obj->update(dt);
	}
}

void GameState::render(sf::RenderTarget &renderTarget) const
{
	for (Object *obj : m_objects)
	{
		obj->render(renderTarget);
	}
}

void GameState::onEnter(GameState *previousState) {}
void GameState::onExit(GameState *nextState) {}

void GameState::addObject(Object *object)
{
	object->setGameState(this);
	m_objects.push_back(object);
}

void GameState::setGameStateController(GameStateController *controller)
{
	m_gameStateController = controller;
}