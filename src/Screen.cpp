#include "Screen.h"

using namespace std;

Screen::Screen(CharacterCore* core)
{
	m_characterCore = core;
}

void Screen::addObject(GameObjectType type, GameObject* object)
{
	m_objects[type].push_back(object);
	object->setScreen(this);
}

vector<GameObject*>* Screen::getObjects(GameObjectType type)
{
	return &m_objects[type];
}

void Screen::onEnter(const Screen* previousScreen)
{
	m_objects = vector<vector<GameObject*>>();
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t+1))
	{
		vector<GameObject*> newVector;
		m_objects.push_back(newVector);
	}
	execOnEnter(previousScreen);
}

void Screen::execOnEnter(const Screen* previousScreen)
{
	// nop
}

void Screen::onExit(const Screen* nextScreen)
{
	deleteAllObjects();
	execOnExit(nextScreen);
}

void Screen::execOnExit(const Screen* nextScreen)
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

const sf::Text* Screen::getTooltipText() const
{
	return &m_tooltipText;
}
	
void Screen::setTooltipText(const std::string& text, const sf::Vector2f& position, const sf::Color& color)
{
	m_tooltipText = sf::Text(
		text,
		(*g_resourceManager->getFont(ResourceID::Font_copperplateGothicBold)));;
	m_tooltipText.setPosition(position);
	m_tooltipText.setColor(color);
}

void Screen::renderTooltipText(sf::RenderTarget& target) const
{
	setViewToTooltipView(target);
	target.draw(m_tooltipText);
}

void Screen::clearTooltipText()
{
	// reset to empty text so tooltip texts don't get stuck
	m_tooltipText = sf::Text();
}

void Screen::setViewToStandardView(sf::RenderTarget& target) const
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, static_cast<float>(WINDOW_HEIGHT) / (WINDOW_HEIGHT + BOTTOM_BORDER)));
	view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	target.setView(view);
}

void Screen::setViewToTooltipView(sf::RenderTarget& target) const
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, BOTTOM_BORDER);
	view.setViewport(sf::FloatRect(0.f, static_cast<float>(WINDOW_HEIGHT) / (WINDOW_HEIGHT + BOTTOM_BORDER), 1.f, static_cast<float>(BOTTOM_BORDER) / (WINDOW_HEIGHT + BOTTOM_BORDER)));
	view.setCenter(WINDOW_WIDTH / 2.f, BOTTOM_BORDER / 2.f);
	target.setView(view);
}

CharacterCore* Screen::getCharacterCore() const
{
	return m_characterCore;
}