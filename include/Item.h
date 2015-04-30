#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class MainCharacter;

// A item cendric can collect
class Item : public GameObject
{
public:
	void loadItem(MainCharacter* mainChar);
	virtual void onRightClick() override;
	virtual void onMouseOver() override;
	void render(sf::RenderTarget &renderTarget) override;

	GameObjectType getConfiguredType() const override;

private:
	MainCharacter* m_mainChar;
};