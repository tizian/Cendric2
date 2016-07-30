#include "GUI/MainWindow.h"
#include "GUI/Toolbar.h"
#include "GUI/TreeWindow.h"
#include "GUI/ErrorWindow.h"
#include "GUI/NodeWindow.h"
#include "ApplicationState.h"

MainWindow::MainWindow() {
	m_toolbar = new Toolbar();
	m_nodeWindow = new NodeWindow();
	m_treeWindow = new TreeWindow(m_nodeWindow);
	m_errorWindow = new ErrorWindow();
	g_state->setErrorWindow(m_errorWindow);
}

MainWindow::~MainWindow() {
	delete m_toolbar;
	delete m_treeWindow;
	delete m_errorWindow;
	delete m_nodeWindow;
}

void MainWindow::update() {
	m_toolbar->update();
	m_treeWindow->update();
	m_errorWindow->update();
	m_nodeWindow->update();

	ImGui::ShowTestWindow();
}

void MainWindow::render() {
	ImGui::Render();
}