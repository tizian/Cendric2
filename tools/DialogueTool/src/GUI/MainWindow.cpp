#include "GUI/MainWindow.h"
#include "GUI/Toolbar.h"
#include "GUI/TreeWindow.h"
#include "GUI/LogWindow.h"
#include "GUI/NodeWindow.h"
#include "GUI/TranslationWindow.h"
#include "ApplicationState.h"

MainWindow::MainWindow() {
	m_translationWindow = new TranslationWindow();
	m_nodeWindow = new NodeWindow(m_translationWindow);
	m_treeWindow = new TreeWindow(m_nodeWindow);
	m_toolbar = new Toolbar(m_nodeWindow);
	m_logWindow = new LogWindow();
	g_state->setLogWindow(m_logWindow);
}

MainWindow::~MainWindow() {
	delete m_toolbar;
	delete m_treeWindow;
	delete m_logWindow;
	delete m_translationWindow;
	delete m_nodeWindow;
}

void MainWindow::update() {
	m_toolbar->update();
	m_treeWindow->update();
	m_logWindow->update();
	m_nodeWindow->update();
	m_translationWindow->update();

	//ImGui::ShowTestWindow();
}

void MainWindow::render() {
	ImGui::Render();
}