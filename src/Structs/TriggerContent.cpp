#include "Structs/TriggerContent.h"
#include "Screens/WorldScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/CutsceneScreen.h"

void TriggerContent::executeTrigger(const TriggerContent& content, WorldScreen* screen) {
	switch (content.type) {
	case TriggerContentType::ConditionProgress:
		screen->notifyConditionAdded(content.s1, content.s2);
		break;
	case TriggerContentType::GoldChange:
		screen->notifyItemChange("gold", content.i1);
		break;
	case TriggerContentType::ItemChange:
		screen->notifyItemChange(content.s1, content.i1);
		break;
	case TriggerContentType::Hint:
		screen->addScreenOverlay(ScreenOverlay::createHintScreenOverlay(content.s1));
		break;
	case TriggerContentType::ItemEquip: {
		auto bean = g_databaseManager->getItemBean(content.s1);
		if (bean.status == BeanStatus::Filled) {
			screen->getCharacterCore()->equipItem(bean.item_id, bean.item_type);
			screen->getInventory()->reload();
		}
		break;
	}
	case TriggerContentType::Weather: {
		WeatherData data;
		data.dimming = content.i1 / 100.f;
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
	case TriggerContentType::Cutscene:
		screen->exitWorld();
		screen->setNextScreen(new CutsceneScreen(screen->getCharacterCore(), content.s1));
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
		if (content.i2 < 1 || content.i2 > 3)
			break;
		SpellModifier modifier;
		modifier.type = type;
		modifier.level = content.i2;
		screen->notifyModifierLearned(modifier);
		break;
	}
	default:
		break;
	}
}

