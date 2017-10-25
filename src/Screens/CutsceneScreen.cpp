#include "Screens/CutsceneScreen.h"
#include "Screens/LoadingScreen.h"

using namespace std;

CutsceneScreen::CutsceneScreen(CharacterCore* core, const std::string& cutsceneID) : Screen(core) {
	m_cutsceneID = cutsceneID;
}

void CutsceneScreen::execOnEnter() {
	m_cutscene = new Cutscene(m_cutsceneID);
	if (!m_cutscene->isLoaded()) {
		delete m_cutscene;
		m_cutscene = nullptr;
		return;
	}

	if (!m_cutscene->getData().mapID.empty()) {
		m_characterCore->setMap(m_cutscene->getData().wordPosition, m_cutscene->getData().mapID);
	}
	else if (!m_cutscene->getData().levelID.empty()) {
		m_characterCore->setLevel(m_cutscene->getData().wordPosition, m_cutscene->getData().levelID);
	}

	g_resourceManager->playMusic(m_cutscene->getData().musicPath, false);
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

void CutsceneScreen::render(sf::RenderTarget& renderTarget) {
	if (m_cutscene == nullptr) return;
	renderTarget.setView(renderTarget.getDefaultView());
	m_cutscene->render(renderTarget);
}

void CutsceneScreen::execOnExit() {
	delete m_cutscene;
}