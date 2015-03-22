#pragma once

#include "stdafx.h"

class Object;
class GameStateController;

class GameState
{
public:
	GameState();
	virtual ~GameState();

	virtual void update(float dt);
	void render(sf::RenderTarget &renderTarget) const;

	virtual void onEnter(GameState *previousState);
	virtual void onExit(GameState *nextState);

	void addObject(Object *object);

	void setGameStateController(GameStateController *controller);
	
private:
	GameStateController *m_gameStateController;
	vector<Object *> m_objects;
};