#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "GUI/SelectableWindow.h"

class WorldInterface;
class ButtonGroup;

class GUITabBar final : public SelectableWindow {
public:
	GUITabBar(WorldInterface* _interface, int size);
	~GUITabBar();

	void show(int index);
	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target) const;
	void update(const sf::Time& frameTime);

	void setPosition(const sf::Vector2f& pos);
	void setButtonTexture(int index, const sf::Texture* tex, int x);
	void setButtonOnClick(int index, const std::function<void()>& agent);
	void setButtonText(int index, const std::string& text);
	void setSelectedElement(int index);

	int getSelectedElement() const;

private:
	WorldInterface* getInterface() const;
	void updateWindowSelected() override;
	void init();

private:
	bool m_isVisible = false;
	int m_size = 0;

	WorldInterface* m_worldInterface;
	ButtonGroup* m_buttonGroup;
	sf::Vector2f m_position;

public:
	static const int WIDTH;
	static const int BUTTON_MARGIN;
};