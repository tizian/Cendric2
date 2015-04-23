#include "Screen.h"

using namespace std;

void Screen::addObject(GameObjectType type, GameObject* object)
{
	m_objects[type].push_back(object);
}

vector<GameObject*>* Screen::getObjects(GameObjectType type)
{
	return &m_objects[type];
}

void Screen::onEnter(Screen* previousScreen)
{
	m_objects = vector<vector<GameObject*>>();
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t+1))
	{
		vector<GameObject*> newVector;
		m_objects.push_back(newVector);
	}
	execOnEnter(previousScreen);
}

void Screen::execOnEnter(Screen* previousScreen)
{
	// nop
}

void Screen::onExit(Screen* nextScreen)
{
	deleteAllObjects();
	execOnExit(nextScreen);
}

void Screen::execOnExit(Screen* nextScreen)
{
	// nop
}

void Screen::deleteDisposedObjects()
{
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1))
	{
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/)
		{
			if ((*it)->isDisposed())
			{
				delete (*it);
				it = m_objects[t].erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void Screen::deleteAllObjects()
{
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1))
	{
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/)
		{
			delete (*it);
			it = m_objects[t].erase(it);
		}
	}
}

void Screen::deleteObjects(GameObjectType type)
{
	for (std::vector<GameObject*>::iterator it = m_objects[type].begin(); it != m_objects[type].end(); /*don't increment here*/)
	{
		delete (*it);
		it = m_objects[type].erase(it);
	}
}

void Screen::updateObjects(GameObjectType type, sf::Time frameTime)
{
	for (std::vector<GameObject*>::iterator it = m_objects[type].begin(); it != m_objects[type].end(); ++it)
	{
		(*it)->update(frameTime);
	}
}

void Screen::renderObjects(GameObjectType type, sf::RenderTarget& renderTarget)
{
	for (std::vector<GameObject*>::iterator it = m_objects[type].begin(); it != m_objects[type].end(); ++it)
	{
		(*it)->render(renderTarget);
	}
}