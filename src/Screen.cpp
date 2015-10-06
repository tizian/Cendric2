#include "Screen.h"
#include "GUI/Button.h"

using namespace std;

#define VIEW_MARGIN 50.f;

inline bool isInsideView(const sf::View& targetView, const sf::FloatRect& boundingBox) {
	sf::FloatRect view(targetView.getCenter().x - targetView.getSize().x / 2.f,
		targetView.getCenter().y - targetView.getSize().y / 2.f,
		targetView.getSize().x,
		targetView.getSize().y);

	view.left -= VIEW_MARGIN;
	view.top -= VIEW_MARGIN;
	view.width += 2 * VIEW_MARGIN;
	view.height += 2 * VIEW_MARGIN;
	return view.intersects(boundingBox);
}

Screen::Screen(CharacterCore* core) {
	m_characterCore = core;

	m_objects = vector<vector<GameObject*>>();
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		vector<GameObject*> newVector;
		m_objects.push_back(newVector);
	}
}

void Screen::addObject(GameObject* object) {
	m_objects[object->getConfiguredType()].push_back(object);
	object->setScreen(this);
}

vector<GameObject*>* Screen::getObjects(GameObjectType type) {
	return &m_objects[type];
}

void Screen::onEnter(const Screen* previousScreen) {
	execOnEnter(previousScreen);
}

void Screen::execOnEnter(const Screen* previousScreen) {
	// nop
}

void Screen::onExit(const Screen* nextScreen) {
	deleteAllObjects();
	execOnExit(nextScreen);
}

void Screen::execOnExit(const Screen* nextScreen) {
	// nop
}

void Screen::deleteDisposedObjects() {
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/) {
			if ((*it)->isDisposed()) {
				delete (*it);
				it = m_objects[t].erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void Screen::setAllButtonsEnabled(bool value) {
	vector<GameObject*>* buttons = getObjects(GameObjectType::_Button);
	for (auto it : *buttons) {
		Button* button = dynamic_cast<Button*>(it);
		if (button != nullptr) {
			button->setEnabled(value);
		}
	}
}

void Screen::deleteAllObjects() {
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/) {
			delete (*it);
			it = m_objects[t].erase(it);
		}
	}
}

void Screen::deleteObjects(GameObjectType type) {
	for (std::vector<GameObject*>::iterator it = m_objects[type].begin(); it != m_objects[type].end(); /*don't increment here*/) {
		delete (*it);
		it = m_objects[type].erase(it);
	}
}

void Screen::updateObjects(GameObjectType type, const sf::Time& frameTime) {
	for (auto& it : m_objects[type]) {
		if (it->isViewable())
			it->update(frameTime);
	}
}

void Screen::renderObjects(GameObjectType type, sf::RenderTarget& renderTarget) {
	for (auto& it : m_objects[type]) {
		it->setViewable(isInsideView(renderTarget.getView(), *(it->getBoundingBox())));
		if (it->isViewable())
			it->render(renderTarget);
	}
}

void Screen::renderObjectsAfterForeground(GameObjectType type, sf::RenderTarget& renderTarget) {
	for (auto &it : m_objects[type]) {
		if (it->isViewable())
			it->renderAfterForeground(renderTarget);
	}
}

const BitmapText* Screen::getTooltipText() const {
	return &m_tooltipText;
}

void Screen::setTooltipText(const wstring& text, const sf::Color& color, bool isOverride) {
	if (m_tooltipTime > sf::Time::Zero && !isOverride) {
		// another text is still displaying
		return;
	}
	m_tooltipText = BitmapText(
		text,
		(*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default)));
	m_tooltipText.setCharacterSize(12);
	m_tooltipText.setPosition(std::max(0.f, (WINDOW_WIDTH - m_tooltipText.getLocalBounds().width) / 2.f), m_isTooltipTop ? 10.f : WINDOW_HEIGHT - m_tooltipText.getLocalBounds().height - 10.f);
	m_tooltipText.setColor(color);
	m_tooltipTime = sf::seconds(1.f + 0.06f * static_cast<float>(text.length()));
}

void Screen::renderTooltipText(sf::RenderTarget& target) const {
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	target.draw(m_tooltipText);
	target.setView(oldView);
}

void Screen::setTooltipPositionTop(bool top) {
	m_isTooltipTop = top;
}

CharacterCore* Screen::getCharacterCore() const {
	return m_characterCore;
}

void Screen::updateTooltipText(const sf::Time& frameTime) {
	if (m_tooltipTime > sf::Time::Zero) {
		m_tooltipTime -= frameTime;
		if (m_tooltipTime <= sf::Time::Zero) // yes, sf::Time can be negative.
		{
			// reset tooltip text
			m_tooltipText = BitmapText();
			m_tooltipTime = sf::Time::Zero;
		}
	}
}

bool Screen::isQuitRequested() const {
	return m_requestQuit;
}