#include "GUI/CharacterInfo.h"
#include "GUI/HintDescriptionWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GlobalResource.h"

const float CharacterInfo::TOP = GUIConstants::TOP;
const float CharacterInfo::LEFT = GUIConstants::LEFT;
const float CharacterInfo::WIDTH = 500.f;
const float CharacterInfo::HEIGHT = GUIConstants::GUI_WINDOW_HEIGHT;

const sf::Vector2f CharacterInfo::BUTTON_SIZE = sf::Vector2f(120.f, 40.f);

const int CharacterInfo::ENTRY_COUNT = 16;
const int CharacterInfo::MAX_ENTRY_LENGTH_CHARACTERS = 34;
const float CharacterInfo::MAX_ENTRY_LENGTH = static_cast<float>(MAX_ENTRY_LENGTH_CHARACTERS) * GUIConstants::CHARACTER_SIZE_M;

const float CharacterInfo::WINDOW_MARGIN = 6.f;

const float CharacterInfo::SCROLL_WINDOW_WIDTH = MAX_ENTRY_LENGTH + 4 * WINDOW_MARGIN + ScrollBar::WIDTH;
const float CharacterInfo::SCROLL_WINDOW_HEIGHT = ENTRY_COUNT * GUIConstants::CHARACTER_SIZE_M + (ENTRY_COUNT - 1) * GUIConstants::CHARACTER_SIZE_M + 2 * GUIConstants::CHARACTER_SIZE_M + 2 * WINDOW_MARGIN;

const float CharacterInfo::SCROLL_WINDOW_TOP = GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y;
const float CharacterInfo::SCROLL_WINDOW_LEFT = 0.5f * (WIDTH - SCROLL_WINDOW_WIDTH);

CharacterInfo::CharacterInfo(const CharacterCore* core, const AttributeData* attributes) {
	m_core = core;
	m_attributes = attributes;

	// init text
	std::string names = "";
	names.append(g_textProvider->getText("Health") + ":\n");
	names.append(g_textProvider->getText("HealthRegeneration") + ":\n");
	names.append(g_textProvider->getText("CriticalHitChance") + ":\n");
	names.append(g_textProvider->getText("Haste") + ":\n\n");

	// dmg 
	names.append(g_textProvider->getText("PhysicalDamage") + ":\n");
	names.append(g_textProvider->getText("FireDamage") + ":\n");
	names.append(g_textProvider->getText("IceDamage") + ":\n");
	names.append(g_textProvider->getText("ShadowDamage") + ":\n");
	names.append(g_textProvider->getText("LightDamage") + ":\n\n");

	// resistance
	names.append(g_textProvider->getText("Armor") + ":\n");
	names.append(g_textProvider->getText("FireResistance") + ":\n");
	names.append(g_textProvider->getText("IceResistance") + ":\n");
	names.append(g_textProvider->getText("ShadowResistance") + ":\n");
	names.append(g_textProvider->getText("LightResistance") + ":\n");

	m_namesText.setString(names);
	m_namesText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_namesText.setColor(COLOR_WHITE);
	m_namesText.setLineSpacing(0.833f);

	m_attributeText.setString("");
	m_attributeText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_attributeText.setColor(COLOR_LIGHT_PURPLE);
	m_attributeText.setLineSpacing(0.833f);

	float yOffset = GUIConstants::TOP + 4 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y;

	m_namesText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + 2.f * GUIConstants::TEXT_OFFSET,
		yOffset));

	m_attributeText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + WIDTH - m_attributeText.getBounds().width - 2.f * GUIConstants::TEXT_OFFSET,
		yOffset));

	m_statsIcons.setPosition(
		GUIConstants::LEFT + WIDTH - m_attributeText.getBounds().width - 2.f * GUIConstants::TEXT_OFFSET - 28.f,
		yOffset - 4.f);

	m_statsIcons.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CHARACTERINFO_ICONS));

	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&CharacterInfo::hide, this));

	delete m_descriptionWindow;
	m_descriptionWindow = new HintDescriptionWindow(m_core);

	// init hint tab section
	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT, m_window);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	reload();

	// init tab bar
	int nTabs = 3;
	float barWidth = nTabs * BUTTON_SIZE.x;
	float barHeight = BUTTON_SIZE.y;
	float x = GUIConstants::LEFT + 0.5f * (m_window->getSize().x - barWidth);
	float y = GUIConstants::TOP + GUIConstants::GUI_TABS_TOP;

	m_tabBar = new TabBar(sf::FloatRect(x, y, barWidth, barHeight), nTabs);
	m_tabBar->getTabButton(0)->setText("Stats");
	m_tabBar->getTabButton(1)->setText("Reputation");
	m_tabBar->getTabButton(2)->setText("Hints");
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}

	// init title
	m_title.setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText("CharacterInfo"));
	m_title.setPosition(
		m_window->getPosition().x +
		m_window->getSize().x / 2 -
		m_title.getLocalBounds().width / 2, m_title.getPosition().y);
}

CharacterInfo::~CharacterInfo() {
	delete m_window;
	delete m_tabBar;
	delete m_descriptionWindow;
	delete m_scrollBar;
	delete m_scrollHelper;
}

bool CharacterInfo::isVisible() const {
	return m_isVisible;
}

void CharacterInfo::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_scrollBar->update(frameTime);

	for (size_t i = 0; i < m_hintEntries.size(); ++i) {
		HintEntry& entry = m_hintEntries[i];
		if (m_selectedEntry && m_selectedEntry->getHintKey() == entry.getHintKey()) {
			entry.setColor(COLOR_WHITE);
		}
		else if (g_inputController->isMouseOver(entry.getBoundingBox(), true)) {
			entry.setColor(COLOR_LIGHT_PURPLE);
		}
		else {
			entry.setColor(COLOR_GREY);
		}
	}

	// check whether an entry was selected
	for (size_t i = 0; i < m_hintEntries.size(); ++i) {
		HintEntry& entry = m_hintEntries[i];
		sf::Vector2f pos = entry.getPosition();
		if (pos.y < TOP + SCROLL_WINDOW_TOP ||
			pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) continue;
		entry.update(frameTime);
		if (entry.isClicked()) {
			selectEntry(&entry);
			return;
		}
	}

	int lastIndex = m_tabBar->getActiveTabIndex();
	m_tabBar->update(frameTime);
	int newIndex = m_tabBar->getActiveTabIndex();
	if (lastIndex != newIndex) {
		hideDescription();
		if (m_selectedEntry != nullptr) {
			m_selectedEntry->deselect();
			m_selectedEntry = nullptr;
		}
		m_scrollBar->setScrollPosition(0.f);
	}

	m_window->update(frameTime);

	if (!m_isReloadNeeded) return;
	reload();
	m_isReloadNeeded = false;

	calculateEntryPositions();
}

void CharacterInfo::calculateEntryPositions() {
	int rows = static_cast<int>(m_hintEntries.size());
	int steps = rows - ENTRY_COUNT + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = 2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M;
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float xOffset = LEFT + SCROLL_WINDOW_LEFT + 2 * WINDOW_MARGIN;
	float yOffset = TOP + SCROLL_WINDOW_TOP + WINDOW_MARGIN + GUIConstants::CHARACTER_SIZE_M - effectiveScrollOffset;

	for (size_t i = 0; i < m_hintEntries.size(); ++i) {
		HintEntry& entry = m_hintEntries[i];
		entry.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		entry.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void CharacterInfo::showDescription(const std::string& hintKey) {
	m_descriptionWindow->reload(hintKey);
	m_descriptionWindow->show();
}

void CharacterInfo::hideDescription() {
	m_descriptionWindow->hide();
	m_selectedHintKey = "";
}

void CharacterInfo::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_title);
	m_tabBar->render(target);

	if (m_tabBar->getActiveTabIndex() == 0) {
		target.draw(m_statsIcons);
		target.draw(m_namesText);
		target.draw(m_attributeText);
	}
	else if (m_tabBar->getActiveTabIndex() == 1) {
		for (auto& text : m_reputationTexts) {
			target.draw(text);
		}
	}
	else {
		for (size_t i = 0; i < m_hintEntries.size(); ++i) {
			m_hintEntries[i].render(m_scrollHelper->texture);
		}
		m_scrollHelper->render(target);

		m_descriptionWindow->render(target);

		target.draw(m_scrollWindow);
		m_scrollBar->render(target);
	}
}

void CharacterInfo::notifyChange() {
	m_isReloadNeeded = true;
}

void CharacterInfo::reload() {
	updateAttributes();
	updateReputation();
	updateHints();
}

void CharacterInfo::updateAttributes() {
	// health
	std::string attributes = "";
	attributes.append(std::to_string(m_attributes->currentHealthPoints));
	attributes.append("/");
	attributes.append(std::to_string(m_attributes->maxHealthPoints));
	attributes.append("\n");

	// health regeneration
	attributes.append(std::to_string(m_attributes->healthRegenerationPerS));
	attributes.append("/s\n");

	// crit
	attributes.append(std::to_string(m_attributes->criticalHitChance));
	attributes.append("%\n");

	// cooldown reduction
	attributes.append(std::to_string(m_attributes->haste));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->cooldownMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("CooldownReduction"));
	attributes.append("\n\n");

	// dmg 
	attributes.append(std::to_string(m_attributes->damagePhysical) + "\n");
	attributes.append(std::to_string(m_attributes->damageFire) + "\n");
	attributes.append(std::to_string(m_attributes->damageIce) + "\n");
	attributes.append(std::to_string(m_attributes->damageShadow) + "\n");
	attributes.append(std::to_string(m_attributes->damageLight) + "\n\n");

	// resistance
	attributes.append(std::to_string(m_attributes->resistancePhysical));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->physicalMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceFire));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->fireMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceIce));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->iceMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceShadow));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->shadowMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceLight));
	//attributes.append(" - ");
	//attributes.append(std::to_string(-round_int(m_attributes->lightMultiplier * 100.f - 100.f)));
	//attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");
	m_attributeText.setString(attributes);

	const sf::Vector2f pos = m_attributeText.getPosition();
	m_attributeText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + WIDTH - m_attributeText.getBounds().width - 2.f * GUIConstants::TEXT_OFFSET,
		pos.y));

	m_statsIcons.setPosition(
		GUIConstants::LEFT + WIDTH - m_attributeText.getBounds().width - 2.f * GUIConstants::TEXT_OFFSET - 28.f,
		pos.y - 4.f);
}

void CharacterInfo::updateReputation() {
	m_reputationTexts.clear();

	float yOffset = GUIConstants::TOP + 3 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y;
	
	if (m_core->getData().reputationProgress.empty()) {
		BitmapText noRep;
		noRep.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		noRep.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		noRep.setColor(COLOR_LIGHT_PURPLE);
		noRep.setString(g_textProvider->getCroppedText("NoReputation", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		m_reputationTexts.push_back(noRep);
		return;
	}
	
	for (auto const& rep : m_core->getData().reputationProgress) {
		BitmapText title;
		title.setString(g_textProvider->getText(EnumNames::getFractionIDName(rep.first)) + ": " + std::to_string(rep.second));
		title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		title.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		title.setColor(COLOR_WHITE);
		m_reputationTexts.push_back(title);

		yOffset += title.getLocalBounds().height * (3 / 2.f);

		BitmapText subtitle;
		std::string key = EnumNames::getFractionIDName(rep.first) + "_" +
			(rep.second >= 75 ? "100" : rep.second >= 50 ? "75" : rep.second >= 25 ? "50" : "25");
		subtitle.setString(g_textProvider->getCroppedText(key, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		subtitle.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		subtitle.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		subtitle.setColor(COLOR_LIGHT_PURPLE);
		m_reputationTexts.push_back(subtitle);

		yOffset += subtitle.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_M;
	}
}

void CharacterInfo::updateHints() {
	m_hintEntries.clear();
	m_selectedEntry = nullptr;

	const std::vector<std::string>& hints = m_core->getData().hintsLearned;

	for (size_t i = 0; i < hints.size(); ++i) {
		m_hintEntries.push_back(HintEntry(hints[i]));
	}

	for (size_t i = 0; i < m_hintEntries.size(); ++i) {
		HintEntry& entry = m_hintEntries[i];
		entry.deselect();
		if (entry.getHintKey().compare(m_selectedHintKey) == 0) {
			selectEntry(&entry);
		}
	}
}

void CharacterInfo::selectEntry(HintEntry* entry) {
	if (entry == nullptr) return;
	if (entry == m_selectedEntry) return;
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
	}
	m_selectedEntry = entry;
	m_selectedEntry->select();
	showDescription(m_selectedEntry->getHintKey());
	m_selectedHintKey = m_selectedEntry->getHintKey();
}

void CharacterInfo::show() {
	m_isVisible = true;
}

void CharacterInfo::hide() {
	m_isVisible = false;
	m_descriptionWindow->hide();
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
		m_selectedEntry = nullptr;
	}
	m_scrollBar->setScrollPosition(0.f);
}

// <<< HINT ENTRY >>>

HintEntry::HintEntry(const std::string& hintKey) {
	m_hintKey = hintKey;
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_name.setColor(COLOR_WHITE);

	std::string hintTitle = "> " + g_textProvider->getText(hintKey, "hint");
	if (hintTitle.size() > CharacterInfo::MAX_ENTRY_LENGTH_CHARACTERS) {
		hintTitle = hintTitle.substr(0, CharacterInfo::MAX_ENTRY_LENGTH_CHARACTERS - 3) + "...";
	}
	m_name.setString(hintTitle);

	setBoundingBox(sf::FloatRect(0.f, 0.f, m_name.getLocalBounds().width, m_name.getLocalBounds().height));
	setInputInDefaultView(true);
}

const std::string& HintEntry::getHintKey() const {
	return m_hintKey;
}

void HintEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
}

void HintEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
}

void HintEntry::onLeftJustPressed() {
	g_inputController->lockAction();
	m_isClicked = true;
}

void HintEntry::setColor(const sf::Color& color) {
	m_name.setColor(color);
}

bool HintEntry::isClicked() {
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void HintEntry::select() {
	m_name.setColor(COLOR_WHITE);
	m_isSelected = true;
}

GameObjectType HintEntry::getConfiguredType() const {
	return GameObjectType::_Interface;
}

void HintEntry::deselect() {
	m_name.setColor(COLOR_GREY);
	m_isSelected = false;
}

bool HintEntry::isSelected() const {
	return m_isSelected;
}