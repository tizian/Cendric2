#include "Screens/CutsceneScreen.h"
#include "Screens/LoadingScreen.h"

using namespace std;

CutsceneScreen::CutsceneScreen(CharacterCore* core, const std::string& cutsceneID) : Screen(core) {
	m_cutsceneID = cutsceneID;
}

void CutsceneScreen::execOnEnter(const Screen* previousScreen) {
	m_cutscene = new Cutscene(m_cutsceneID);
	if (!m_cutscene->isLoaded()) {
		delete m_cutscene;
		m_cutscene = nullptr;
	}
}

void CutsceneScreen::execUpdate(const sf::Time& frameTime) {
	if (m_cutscene == nullptr) {
		setNextScreen(new LoadingScreen(m_characterCore));
		return;
	}
	m_cutscene->update(frameTime);
	if (m_cutscene->isNoStepsLeft()) {
		setNextScreen(new LoadingScreen(m_characterCore));
	}
}

void CutsceneScreen::render(sf::RenderTarget &renderTarget) {
	if (m_cutscene == nullptr) return;
	m_cutscene->render(renderTarget);
}

void CutsceneScreen::execOnExit(const Screen *nextScreen) {
	delete m_cutscene;
}