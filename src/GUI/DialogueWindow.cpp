#include "GUI/DialogueWindow.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

DialogueWindow::~DialogueWindow()
{
	delete m_dialogueText;
	delete m_cendricSprite;
	delete m_cendricText;
	delete m_npcSprite;
	delete m_npcText;
}

void DialogueWindow::setNPC(NpcID npc)
{
	delete m_cendricSprite;
	delete m_cendricText;
	delete m_npcSprite;
	delete m_npcText;

	// TODO
}

void DialogueWindow::render(sf::RenderTarget& renderTarget)
{
	Window::render(renderTarget);
}

void DialogueWindow::setPosition(const sf::Vector2f& position)
{
	Window::setPosition(position);
}