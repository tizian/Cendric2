#include "Steam/Achievements/GemAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_MODIFIERS, GemAchievement)

bool GemAchievement::notifyCore(const CharacterCore* core) {
	auto const& gems = core->getData().modfiersLearned;

	return (
		contains(gems, SpellModifierType::Count) && gems.at(SpellModifierType::Count) == 3 &&
		contains(gems, SpellModifierType::Duration) && gems.at(SpellModifierType::Duration) == 3 &&
		contains(gems, SpellModifierType::Range) && gems.at(SpellModifierType::Range) == 3 &&
		contains(gems, SpellModifierType::Reflect) && gems.at(SpellModifierType::Reflect) == 3 &&
		contains(gems, SpellModifierType::Speed) && gems.at(SpellModifierType::Speed) == 3 &&
		contains(gems, SpellModifierType::Strength) && gems.at(SpellModifierType::Strength) == 3)
}
