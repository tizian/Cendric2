#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.loadFromFile(g_resourceManager->getFilename(ResourceID::Level_testLevel));
	MainCharacter* mainChar = new MainCharacter();
	mainChar->load();
	addObject(mainChar);
	cout << "...done." << endl;
}

Screen* GameScreen::update(sf::Time frameTime)
{
	for (Object *obj : m_objects)
	{
		obj->update(frameTime);
	}
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget) const
{
	m_currentLevel.draw(renderTarget, RenderStates::Default);
	for (Object *obj : m_objects)
	{
		obj->render(renderTarget);
	}
}