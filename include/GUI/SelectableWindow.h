#pragma once

#include "global.h"

class Window;

class SelectableWindow {
public:
	SelectableWindow() = default;
	virtual ~SelectableWindow() {};

	void setRightWindow(SelectableWindow* rightWindow);
	void setLeftWindow(SelectableWindow* leftWindow);

	void setWindowSelected(bool isSelected);

	void setRightWindowSelected();
	void setLeftWindowSelected();

protected:
	virtual void updateWindowSelected() = 0;
	bool isWindowSelected() const;

	Window* m_window = nullptr;

private:
	void updateWindowColor() const;

	SelectableWindow* m_rightWindow = nullptr;
	SelectableWindow* m_leftWindow = nullptr;

	bool m_isWindowSelected = false;
};