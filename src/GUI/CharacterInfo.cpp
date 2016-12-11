#include "GUI/CharacterInfo.h"
#include "GUI/HintDescriptionWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GlobalResource.h"
#include "Screens/WorldScreen.h"

static const std::string GODMODE_CHEAT = "god";

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

const std::vector<std::string> CharacterInfo::LABELS = {
	"Health",
	"HealthRegeneration",
	"CriticalHitChance",
	"Haste",
	"",
	"PhysicalDamage",
	"FireDamage",
	"IceDamage",
	"ShadowDamage",
	"LightDamage",
	"",
	"Armor",
	"FireResistance",
	"IceResistance",
	"ShadowResistance",
	"LightResistance",
};

CharacterInfo::CharacterInfo(WorldScreen* screen, const AttributeData* attributes) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
	m_attributes = attributes;

	BitmapText name;
	name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	name.setColor(COLOR_WHITE);
	name.setString("Placeholder");

	sf::Sprite icon;
	icon.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CHARACTERINFO_ICONS));

	float yOffset = GUIConstants::TOP + GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y + GUIConstants::CHARACTER_SIZE_M;
	float textHeight = name.getLocalBounds().height;
	float dy = textHeight + GUIConstants::CHARACTER_SIZE_M;

	for (size_t i = 0; i < LABELS.size(); ++i) {
		if (LABELS[i].compare("") == 0) {
			yOffset += dy;
			continue;
		}

		name.setPosition(LEFT + 2 * GUIConstants::TEXT_OFFSET, yOffset);
		name.setString(g_textProvider->getText(LABELS[i]) + ":");
		m_nameTexts.push_back(name);

		icon.setTextureRect(sf::IntRect(0, static_cast<int>(i) * 18, 18, 18));
		icon.setPosition(sf::Vector2f(LEFT + WIDTH - 160.f, yOffset + 0.5f * (textHeight - 18)));
		m_statIcons.push_back(icon);

		yOffset += dy;
	}

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
		for (auto& text : m_nameTexts) {
			target.draw(text);
		}
		for (auto& text : m_attributeTexts) {
			target.draw(text);
		}
		for (auto& icon : m_statIcons) {
			target.draw(icon);
		}
	}
	else if (m_tabBar->getActiveTabIndex() == 1) {
		target.draw(m_guild);
		target.draw(m_guildSprite);
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
	m_attributeTexts.clear();

	std::vector<std::string> attributes;

	// health
	std::string health = std::to_string(m_attributes->currentHealthPoints) +
		"/" +
		std::to_string(m_attributes->maxHealthPoints);
	attributes.push_back(health);

	// health regeneration
	std::string healthRegen = std::to_string(m_attributes->healthRegenerationPerS)
		+ "/s";
	attributes.push_back(healthRegen);

	// crit
	std::string crit = std::to_string(m_attributes->criticalHitChance) +
		"%";
	attributes.push_back(crit);

	// cooldown reduction
	attributes.push_back(std::to_string(m_attributes->haste));

	attributes.push_back("");

	// dmg 
	attributes.push_back(std::to_string(m_attributes->damagePhysical));
	attributes.push_back(std::to_string(m_attributes->damageFire));
	attributes.push_back(std::to_string(m_attributes->damageIce));
	attributes.push_back(std::to_string(m_attributes->damageShadow));
	attributes.push_back(std::to_string(m_attributes->damageLight));

	attributes.push_back("");

	// resistance
	attributes.push_back(std::to_string(m_attributes->resistancePhysical));
	attributes.push_back(std::to_string(m_attributes->resistanceFire));
	attributes.push_back(std::to_string(m_attributes->resistanceIce));
	attributes.push_back(std::to_string(m_attributes->resistanceShadow));
	attributes.push_back(std::to_string(m_attributes->resistanceLight));

	BitmapText attributeText;
	attributeText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	attributeText.setColor(COLOR_WHITE);
	attributeText.setString("Placeholder");

	float yOffset = GUIConstants::TOP + GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y + GUIConstants::CHARACTER_SIZE_M;
	float textHeight = attributeText.getLocalBounds().height;
	float dy = textHeight + GUIConstants::CHARACTER_SIZE_M;

	for (size_t i = 0; i < attributes.size(); ++i) {
		if (attributes[i].compare("") == 0) {
			yOffset += dy;
			continue;
		}

		attributeText.setPosition(LEFT + WIDTH - 136.f, yOffset);
		attributeText.setString(attributes[i]);
		m_attributeTexts.push_back(attributeText);

		yOffset += dy;
	}
}

void CharacterInfo::updateReputation() {
	m_reputationTexts.clear();

	float yOffset = GUIConstants::TOP + GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y + GUIConstants::CHARACTER_SIZE_M;
	float xOffset = 3 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT;

	std::string currentGuild;
	currentGuild.append(g_textProvider->getText("Guild"));
	currentGuild.append(": ");
	currentGuild.append(g_textProvider->getText(EnumNames::getFractionIDName(m_core->getData().guild)));
	m_guild.setString(currentGuild);
	m_guild.setPosition(LEFT + (WIDTH - m_guild.getLocalBounds().width) * 0.5f, yOffset);
	m_guild.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	yOffset += 2 * GUIConstants::CHARACTER_SIZE_M;
	
	auto* tex = g_resourceManager->getTexture(GlobalResource::TEX_GUILDICONS);
	int texHeight = 100;
	if (tex != nullptr)
		m_guildSprite.setTexture(*tex);
	m_guildSprite.setPosition(LEFT + (WIDTH - 100) * 0.5f, yOffset);
	if (m_core->getData().guild == FractionID::VOID) {
		m_guildSprite.setTextureRect(sf::IntRect());
	}
	else {
		m_guildSprite.setTextureRect(sf::IntRect(texHeight * (static_cast<int>(m_core->getData().guild) - 1), 0, texHeight, texHeight));
		yOffset += texHeight;
	}

	yOffset += 2 * GUIConstants::CHARACTER_SIZE_M;

	if (m_core->getData().reputationProgress.empty()) {
		BitmapText noRep;
		noRep.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		noRep.setPosition(xOffset, yOffset);
		noRep.setColor(COLOR_LIGHT_PURPLE);
		noRep.setString(g_textProvider->getCroppedText("NoReputation", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		m_reputationTexts.push_back(noRep);
		return;
	}

	for (auto const& rep : m_core->getData().reputationProgress) {
		BitmapText title;
		title.setString(g_textProvider->getText(EnumNames::getFractionIDName(rep.first)) + ": " + std::to_string(rep.second));
		title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		title.setPosition(xOffset, yOffset);
		title.setColor(COLOR_WHITE);
		m_reputationTexts.push_back(title);

		yOffset += title.getLocalBounds().height * (3 / 2.f);

		BitmapText subtitle;
		std::string key = EnumNames::getFractionIDName(rep.first) + "_" +
			(rep.second >= 75 ? "100" : rep.second >= 50 ? "75" : rep.second >= 25 ? "50" : "25");
		subtitle.setString(g_textProvider->getCroppedText(key, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		subtitle.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		subtitle.setPosition(xOffset, yOffset);
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
	g_inputController->startReadingText();
}

void CharacterInfo::hide() {
	m_isVisible = false;
	m_descriptionWindow->hide();
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
		m_selectedEntry = nullptr;
	}
	m_scrollBar->setScrollPosition(0.f);

	// handle GODMODE
	std::string read = g_inputController->getReadText();
	if (!read.empty()) {
		read.pop_back();
		if (GODMODE_CHEAT.compare(read) == 0) {
			m_screen->toggleGodmode();
		}
	}
	
	g_inputController->stopReadingText();
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