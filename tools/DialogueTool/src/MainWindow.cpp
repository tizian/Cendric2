#include "MainWindow.h"
#include "Toolbar.h"

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