#include "Structs/TriggerContent.h"
#include "Screens/WorldScreen.h"

void TriggerContent::executeTrigger(const TriggerContent& content, WorldScreen* screen) {
	switch (content.type) {
	case TriggerContentType::ConditionProgress:
		screen->notifyConditionAdded(content.firstStringAttribute, content.secondStringAttribute);
		break;
	case TriggerContentType::GoldChange:
		screen->notifyItemChange("gold", content.integerAttribute);
		break;
	case TriggerContentType::ItemChange:
		screen->notifyItemChange(content.firstStringAttribute, content.integerAttribute);
		break;
	case TriggerContentType::Hint:
		screen->addScreenOverlay(ScreenOverlay::createHintScreenOverlay(content.firstStringAttribute));
		break;
	case TriggerContentType::ItemEquip: {
		auto bean = g_databaseManager->getItemBean(content.firstStringAttribute);
		if (bean.status == BeanStatus::Filled) {
			screen->getCharacterCore()->equipItem(bean.item_id, bean.item_type);
			screen->getInventory()->reload();
		}
		break;
	}
	case TriggerContentType::QuestConditionProgress:
		screen->notifyQuestConditionFulfilled(content.firstStringAttribute, content.secondStringAttribute);
		break;
	case TriggerContentType::QuestDescriptionProgress:
		screen->notifyQuestDescriptionAdded(content.firstStringAttribute, content.integerAttribute);
		break;
	case TriggerContentType::QuestStateChange:
		screen->notifyQuestStateChanged(content.firstStringAttribute, static_cast<QuestState>(content.integerAttribute));
		break;
	default:
		break;
	}
}

