#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

#include "Enums/NpcID.h"

class DialogueWindow : public Window
{
public:
	~DialogueWindow();

	void render(sf::RenderTarget& renderTarget) override;
	void setNPC(NpcID id);
	void setPosition(const sf::Vector2f& position) override;

private:
	BitmapText* m_dialogueText = nullptr;
	BitmapText* m_npcText = nullptr;
	sf::Sprite* m_npcSprite = nullptr;
	BitmapText* m_cendricText = nullptr;
	sf::Sprite* m_cendricSprite = nullptr;
};