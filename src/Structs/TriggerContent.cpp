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
	case TriggerContentType::QuestConditionProgress:
		screen->notifyQuestConditionFulfilled(content.s1, content.s2);
		break;
	case TriggerContentType::QuestDescriptionProgress:
		screen->notifyQuestDescriptionAdded(content.s1, content.i1);
		break;
	case TriggerContentType::QuestStateChange:
		screen->notifyQuestStateChanged(content.s1, static_cast<QuestState>(content.i1));
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
	default:
		break;
	}
}

