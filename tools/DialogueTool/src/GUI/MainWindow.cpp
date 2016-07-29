#include "GUI/MainWindow.h"
#include "GUI/Toolbar.h"
#include "GUI/TreeWindow.h"

MainWindow::MainWindow() {
	m_toolbar = new Toolbar();
}

MainWindow::~MainWindow() {
	delete m_toolbar;
}

void MainWindow::update() {
	m_toolbar->update();
}

void MainWindow::render() {
	ImGui::Render();
}