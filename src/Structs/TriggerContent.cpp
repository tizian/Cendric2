#include "Structs/TriggerContent.h"
#include "Screens/WorldScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/CutsceneScreen.h"

void TriggerContent::executeTrigger(const TriggerContent& content, WorldScreen* screen) {
	if (!content.isValid() || screen == nullptr) return;
	switch (content.type) {
	case TriggerContentType::ConditionProgress: {
		Condition cond;
		cond.type = content.s1;
		cond.name = content.s2;
		screen->notifyConditionAdded(cond);
		break;
	}
	case TriggerContentType::GoldChange:
		screen->notifyItemChange("gold", content.i1);
		break;
	case TriggerContentType::ItemChange:
		screen->notifyItemChange(content.s1, content.i1);
		break;
	case TriggerContentType::Hint:
		screen->notifyHintAdded(content.s1);
		break;
	case TriggerContentType::ItemEquip: {
		screen->notifyItemEquip(content.s1);
		break;
	}
	case TriggerContentType::Weather: {
		WeatherData data;
		data.ambientDimming = content.i1 / 100.f;
		data.weather = content.s2;
		screen->getCharacterCore()->setWeather(content.s1, data);
		break;
	}
	case TriggerContentType::QuestConditionProgress:
		screen->notifyQuestConditionFulfilled(content.s1, content.s2);
		break;
	case TriggerContentType::QuestDescriptionProgress:
		screen->notifyQuestDescriptionAdded(content.s1, content.i1);
		break;
	case TriggerContentType::QuestStateChange:
		screen->notifyQuestStateChanged(content.s1, static_cast<QuestState>(content.i1));
		break;
	case TriggerContentType::ReputationProgress:
		screen->notifyReputationAdded(static_cast<FractionID>(content.i1), content.i2);
		break;
	case TriggerContentType::SetGuild:
		screen->notifyGuildSet(static_cast<FractionID>(content.i1));
		break;
	case TriggerContentType::MapEntry:
		screen->exitWorld();
		screen->getCharacterCore()->setMap(sf::Vector2f(static_cast<float>(content.i1), static_cast<float>(content.i2)), content.s1);
		screen->setNextScreen(new LoadingScreen(screen->getCharacterCore()));
		break;
	case TriggerContentType::LevelEntry:
		screen->exitWorld();
		screen->getCharacterCore()->setLevel(sf::Vector2f(static_cast<float>(content.i1), static_cast<float>(content.i2)), content.s1);
		screen->setNextScreen(new LoadingScreen(screen->getCharacterCore()));
		break;
	case TriggerContentType::SetMap:
		screen->getCharacterCore()->setMap(sf::Vector2f(static_cast<float>(content.i1), static_cast<float>(content.i2)), content.s1);
		break;
	case TriggerContentType::SetLevel:
		screen->getCharacterCore()->setLevel(sf::Vector2f(static_cast<float>(content.i1), static_cast<float>(content.i2)), content.s1);
		break;
	case TriggerContentType::SetForcedMap:
		screen->getCharacterCore()->setForcedMap(sf::Vector2f(static_cast<float>(content.i1), static_cast<float>(content.i2)), content.s1);
		break;
	case TriggerContentType::Cutscene:
		screen->exitWorld();
		screen->setNextScreen(new CutsceneScreen(screen->getCharacterCore(), content.s1));
		break;
	case TriggerContentType::AchievementUnlocked:
		screen->getCharacterCore()->notifyAchievementUnlocked(content.s1);
		break;
	case TriggerContentType::LearnSpell: {
		SpellID id = static_cast<SpellID>(content.i1);
		if (id <= SpellID::VOID || id >= SpellID::MAX) 
			break;
		screen->notifySpellLearned(id);
		break;
	}
	case TriggerContentType::LearnModifier: {
		SpellModifierType type = static_cast<SpellModifierType>(content.i1);
		if (type <= SpellModifierType::VOID || type >= SpellModifierType::MAX)
			break;
		screen->notifyModifierLearned(type, -1);
		break;
	}
	default:
		break;
	}
}

TriggerContent TriggerContent::changeQuestState(const std::string& questID, const std::string& state) {
	if (questID.empty()) {
		g_logger->logError("TriggerContent", "Quest ID cannot be empty.");
		return TriggerContent();
	}
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::VOID || questState == QuestState::MAX) {
		g_logger->logError("TriggerContent", "Cannot change state: Quest state [" + state + "] does not exist.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::QuestStateChange);
	content.s1 = questID;
	content.i1 = static_cast<int>(questState);
	return content;
}

TriggerContent TriggerContent::addQuestDescription(const std::string& questID, int descriptionID) {
	if (questID.empty()) {
		g_logger->logError("TriggerContent", "Quest ID cannot be empty.");
		return TriggerContent();
	}
	if (descriptionID < 1) {
		g_logger->logError("TriggerContent", "Description ID must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::QuestDescriptionProgress);
	content.s1 = questID;
	content.i1 = descriptionID;
	return content;
}

TriggerContent TriggerContent::addQuestProgress(const std::string& questID, const std::string& progress) {
	if (questID.empty() || progress.empty()) {
		g_logger->logError("TriggerContent", "Quest ID and quest progress cannot be empty.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::QuestConditionProgress);
	content.s1 = questID;
	content.s2 = progress;
	return content;
}

TriggerContent TriggerContent::addConditionProgress(const std::string& conditionType, const std::string& condition) {
	if (condition.empty() || conditionType.empty()) {
		g_logger->logError("TriggerContent", "Condition and condition type cannot be empty.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::ConditionProgress);
	content.s1 = conditionType;
	content.s2 = condition;
	return content;
}

TriggerContent TriggerContent::addReputationProgress(const std::string& fractionID, int amount) {
	FractionID fraction = resolveFractionID(fractionID);
	if (fraction == FractionID::VOID || amount < 0) {
		g_logger->logError("TriggerContent", "Fraction id not recognized or reputation amount < 0");
		return TriggerContent();
	}

	TriggerContent content(TriggerContentType::ReputationProgress);
	content.i1 = static_cast<int>(fraction);
	content.i2 = amount;
	return content;
}

TriggerContent TriggerContent::addHint(const std::string& hint) {
	if (hint.empty()) {
		g_logger->logError("TriggerContent", "Hint cannot be empty.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::Hint);
	content.s1 = hint;
	return content;
}

TriggerContent TriggerContent::addItem(const std::string& itemID, int amount) {
	if (itemID.empty()) {
		g_logger->logError("TriggerContent", "Item ID cannot be empty.");
		return TriggerContent();
	}
	if (amount <= 0) {
		g_logger->logError("TriggerContent", "amount cannot be negative.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::ItemChange);
	content.s1 = itemID;
	content.i1 = amount;
	return content;
}

TriggerContent TriggerContent::equipItem(const std::string& itemID) {
	if (itemID.empty()) {
		g_logger->logError("TriggerContent", "Item ID cannot be empty.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::ItemEquip);
	content.s1 = itemID;
	return content;
}

TriggerContent TriggerContent::removeItem(const std::string& itemID, int amount) {
	if (itemID.empty()) {
		g_logger->logError("TriggerContent", "Item ID cannot be empty.");
		return TriggerContent();
	}
	if (amount <= 0) {
		g_logger->logError("TriggerContent", "amount cannot be negative.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::ItemChange);
	content.s1 = itemID;
	content.i1 = -amount;
	return content;
}

TriggerContent TriggerContent::addGold(int amount) {
	if (amount <= 0) {
		g_logger->logError("TriggerContent", "amount cannot be negative.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::GoldChange);
	content.i1 = amount;
	return content;
}

TriggerContent TriggerContent::removeGold(int amount) {
	if (amount <= 0) {
		g_logger->logError("TriggerContent", "amount cannot be negative.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::GoldChange);
	content.i1 = -amount;
	return content;
}

TriggerContent TriggerContent::learnSpell(int id) {
	SpellID spellID = static_cast<SpellID>(id);
	if (spellID <= SpellID::VOID || spellID >= SpellID::MAX) {
		g_logger->logError("TriggerContent", "Cannot learn spell, spell ID not recognized.");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::LearnSpell);
	content.i1 = id;
	return content;
}

TriggerContent TriggerContent::setGuild(const std::string& guild) {
	FractionID fractionID = resolveFractionID(guild);
	if (fractionID == FractionID::VOID) {
		g_logger->logError("TriggerContent", "Cannot set guild, guild id not recognized: " + guild);
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::SetGuild);
	content.i1 = static_cast<int>(fractionID);
	return content;
}

TriggerContent TriggerContent::startLevel(const std::string& levelID, int x, int y) {
	if (levelID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("TriggerContent", "Level ID cannot be empty and the spawn position (x and y) must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::LevelEntry);
	content.s1 = levelID;
	content.i1 = x;
	content.i2 = y;
	return content;
}

TriggerContent TriggerContent::startMap(const std::string& mapID, int x, int y) {
	if (mapID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("TriggerContent", "Map ID cannot be empty and the spawn position (x and y) must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::MapEntry);
	content.s1 = mapID;
	content.i1 = x;
	content.i2 = y;
	return content;
}

TriggerContent TriggerContent::setLevel(const std::string& levelID, int x, int y) {
	if (levelID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("TriggerContent", "Level ID cannot be empty and the spawn position (x and y) must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::SetLevel);
	content.s1 = levelID;
	content.i1 = x;
	content.i2 = y;
	return content;
}

TriggerContent TriggerContent::setMap(const std::string& mapID, int x, int y) {
	if (mapID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("TriggerContent", "Map ID cannot be empty and the spawn position (x and y) must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::SetMap);
	content.s1 = mapID;
	content.i1 = x;
	content.i2 = y;
	return content;
}

TriggerContent TriggerContent::setForcedMap(const std::string& mapID, int x, int y) {
	if (mapID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("TriggerContent", "Map ID cannot be empty and the spawn position (x and y) must be > 0");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::SetForcedMap);
	content.s1 = mapID;
	content.i1 = x;
	content.i2 = y;
	return content;
}

TriggerContent TriggerContent::startCutscene(const std::string& cutsceneID) {
	if (cutsceneID.empty()) {
		g_logger->logError("TriggerContent", "Cutscene ID cannot be empty");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::Cutscene);
	content.s1 = cutsceneID;
	return content;
}

TriggerContent TriggerContent::unlockAchievement(const std::string& achievement) {
	if (achievement.empty()) {
		g_logger->logError("TriggerContent", "Achievement ID cannot be empty");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::AchievementUnlocked);
	content.s1 = achievement;
	return content;
}

TriggerContent TriggerContent::setWeather(const std::string& worldId, const std::string& weather, int dimming) {
	if (worldId.empty()) {
		g_logger->logError("TriggerContent", "World Id cannot be empty");
		return TriggerContent();
	}
	TriggerContent content(TriggerContentType::Weather);
	content.i1 = clamp(dimming, 0, 100);
	content.s1 = worldId;
	content.s2 = weather;
	return content;
}

bool TriggerContent::isValid() const {
	return type > TriggerContentType::VOID && type < TriggerContentType::MAX;
}

