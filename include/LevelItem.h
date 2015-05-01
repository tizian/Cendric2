#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class MainCharacter;

enum class LevelItemID
{
	Void,
	Food_Cheese,
	Food_Bread,
	Food_Water
};

// A item cendric can collect in a level
class LevelItem : public GameObject
{
public:
	void loadItem(MainCharacter* mainChar);
	virtual void onRightClick() override;
	virtual void onMouseOver() override;
	void render(sf::RenderTarget &renderTarget) override;

	GameObjectType getConfiguredType() const override;
	virtual LevelItemID getConfiguredItemID() const = 0;
	virtual const std::string& getConfiguredTooltipText() const = 0;

private:
	MainCharacter* m_mainChar;
};