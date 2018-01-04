#include "Steam/Achievements/GemAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_MODIFIERS, GemAchievement)

bool GemAchievement::notifyCore(const CharacterCore* core) {
	auto const& gems = core->getData().modfiersLearned;

	for (auto t = static_cast<int>(SpellModifierType::VOID) + 1; t < static_cast<int>(SpellModifierType::MAX); ++t) {
		auto type = static_cast<SpellModifierType>(t);
		if (!contains(gems, type) || gems.at(type) < 3) {
			return false;
		}
	}

	return true;
}
