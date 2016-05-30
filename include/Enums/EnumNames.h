#pragma once

#include "global.h"

#include "Enums/Language.h"
#include "Enums/Key.h"
#include "Enums/SpellType.h"
#include "Enums/SpellID.h"
#include "Structs/SpellModifier.h"
#include "Enums/QuestState.h"
#include "Enums/DamageType.h"
#include "Enums/FractionID.h"
#include "Enums/ItemType.h"
#include "Enums/EnemyID.h"

// pure static class used to resolve enum names.
class EnumNames {
public:
	static std::string getLanguageName(Language lang) {
		switch (lang) {
		default:
		case Language::MAX:
		case Language::VOID:
			return "Unknown";
		case Language::Lang_CH:
			return "SwissGerman";
		case Language::Lang_DE:
			return "German";
		case Language::Lang_EN:
			return "English";
		}
	}

	static std::string getItemTypeName(ItemType type) {
		switch (type) {
		default:
		case ItemType::MAX:
		case ItemType::VOID:
			return "Unknown";
		case ItemType::Consumable:
			return "Consumable";
		case ItemType::Convertible:
			return "Convertible";
		case ItemType::Document:
			return "Document";
		case ItemType::Equipment_back:
			return "EqBack";
		case ItemType::Equipment_body:
			return "EqBody";
		case ItemType::Equipment_head:
			return "EqHead";
		case ItemType::Equipment_neck:
			return "EqNeck";
		case ItemType::Equipment_ring_1:
		case ItemType::Equipment_ring_2:
			return "EqRing";
		case ItemType::Equipment_weapon:
			return "EqWeapon";
		case ItemType::Gold:
			return "Gold";
		case ItemType::Misc:
			return "Miscellaneous";
		case ItemType::Permanent:
			return "Permanent";
		case ItemType::Quest:
			return "Quest";
		}
	}

	static std::string getFractionIDName(FractionID id) {
		switch (id) {
		default:
		case FractionID::MAX:
		case FractionID::VOID:
			return "Unknown";
		case FractionID::Cleric:
			return "cleric";
		case FractionID::Druid:
			return "druid";
		case FractionID::Elementalist:
			return "elementalist";
		case FractionID::Necromancer:
			return "necromancer";
		case FractionID::Thief:
			return "thief";
		}
	}

	static std::string getDamageTypeName(DamageType type) {
		switch (type) {
		default:
		case DamageType::MAX:
		case DamageType::VOID:
			return "Unknown";
		case DamageType::Physical:
			return "PhysicalDamage";
		case DamageType::Fire:
			return "FireDamage";
		case DamageType::Ice:
			return "IceDamage";
		case DamageType::Light:
			return "LightDamage";
		case DamageType::Shadow:
			return "ShadowDamage";
		}
	}

	static std::string getQuestStateName(QuestState state) {
		switch (state) {
		default:
		case QuestState::MAX:
		case QuestState::VOID:
			return "Unknown";
		case QuestState::Started:
			return "Active";
		case QuestState::Failed:
			return "Failed";
		case QuestState::Completed:
			return "Completed";
		}
	}

	static std::string getSpellTypeName(SpellType type) {
		switch (type) {
		default:
		case SpellType::MAX:
		case SpellType::VOID:
			return "Unknown";
		case SpellType::Elemental:
			return "Elemental";
		case SpellType::Divine:
			return "Divine";
		case SpellType::Twilight:
			return "Twilight";
		case SpellType::Necromancy:
			return "Necromancy";
		case SpellType::Meta:
			return "Meta";
		}
	}

	static std::string getSpellIDName(SpellID id) {
		switch (id) {
		default:
		case SpellID::MAX:
		case SpellID::VOID:
			return "Unknown";
		case SpellID::Chop:
			return "Chop";
		case SpellID::AntiGravity:
			return "SpellAntiGravity";
		case SpellID::Aureola:
			return "SpellAureola";
		case SpellID::DivineShield:
			return "SpellDivineShield";
		case SpellID::Fear:
			return "SpellFear";
		case SpellID::FireBall:
			return "SpellFireBall";
		case SpellID::Flash:
			return "SpellFlash";
		case SpellID::GhostForm:
			return "SpellGhostForm";
		case SpellID::IceBall:
			return "SpellIceBall";
		case SpellID::IcyAmbush:
			return "SpellIcyAmbush";
		case SpellID::Invisibility:
			return "SpellInvisibility";
		case SpellID::LeapOfFaith:
			return "SpellLeapOfFaith";
		case SpellID::Leech:
			return "SpellLeech";
		case SpellID::Light:
			return "SpellLight";
		case SpellID::RaiseTheDead:
			return "SpellRaiseTheDead";
		case SpellID::ShadowTrap:
			return "SpellShadowTrap";
		case SpellID::SummonGargoyle:
			return "SpellSummonGargoyle";
		case SpellID::Telekinesis:
			return "SpellTelekinesis";
		case SpellID::Unlock:
			return "SpellUnlock";
		case SpellID::WindGust:
			return "SpellWindGust";
		case SpellID::HolyFire:
			return "SpellHolyFire";
		}
	}

	static std::string getSpellModifierTypeName(SpellModifierType type) {
		switch (type) {
		default:
		case SpellModifierType::MAX:
		case SpellModifierType::VOID:
			return "Unknown";
		case SpellModifierType::Count:
			return "Count";
		case SpellModifierType::Duration:
			return "Duration";
		case SpellModifierType::Range:
			return "Range";
		case SpellModifierType::Reflect:
			return "Reflection";
		case SpellModifierType::Speed:
			return "Speed";
		case SpellModifierType::Strength:
			return "Strength";
		}
	}

	static std::string getEnemyName(EnemyID enemy) {
		switch (enemy) {
		default:
		case EnemyID::MAX:
		case EnemyID::VOID:
			return "Unknown";
		case EnemyID::Rat:
			return "Rat";
		case EnemyID::FireRat:
			return "FireRat";
		case EnemyID::Nekomata:
			return "Nekomata";
		case EnemyID::Crow:
			return "Crow";
		case EnemyID::Skeleton:
			return "Skeleton";
		case EnemyID::Gargoyle:
			return "Gargoyle";
		case EnemyID::Hunter:
			return "Hunter";
		case EnemyID::Wisp:
			return "Wisp";
		case EnemyID::Seagull:
			return "Seagull";
		case EnemyID::Wolf:
			return "Wolf";
		case EnemyID::Stoneman:
			return "Stoneman";
		case EnemyID::Boss_Zeff:
			return "BossZeff";
		}
	}

	static std::string getKeyName(Key key) {
		switch (key) {
		default:
		case Key::MAX:
		case Key::VOID:
			return "Unknown";
		case Key::CharacterInfo:
			return "CharacterInfo";
		case Key::Down:
			return "MoveDown";
		case Key::Escape:
			return "Exit";
		case Key::Inventory:
			return "Inventory";
		case Key::Map:
			return "Map";
		case Key::Spellbook:
			return "Spellbook";
		case Key::Interact:
			return "Interact";
		case Key::Jump:
			return "Jump";
		case Key::Left:
			return "MoveLeft";
		case Key::Quickload:
			return "Quickload";
		case Key::Quicksave:
			return "Quicksave";
		case Key::Journal:
			return "Journal";
		case Key::Confirm:
			return "Confirm";
		case Key::Right:
			return "MoveRight";
		case Key::Chop:
			return "Chop";
		case Key::FirstSpell:
			return "FirstSpell";
		case Key::SecondSpell:
			return "SecondSpell";
		case Key::ThirdSpell:
			return "ThirdSpell";
		case Key::FourthSpell:
			return "FourthSpell";
		case Key::FifthSpell:
			return "FifthSpell";
		case Key::QuickSlot1:
			return "Quickslot1";
		case Key::QuickSlot2:
			return "Quickslot2";
		case Key::Up:
			return "MoveUp";
		case Key::ToggleTooltips:
			return "ToggleTooltips";
		case Key::Screenshot:
			return "Screenshot";
		}
	}

	static std::string getKeyboardKeyName(sf::Keyboard::Key key) {
		switch (key) {
		default:
		case sf::Keyboard::Unknown:
			return "Unknown";
		case sf::Keyboard::KeyCount:
			return "<...>";
		case sf::Keyboard::A:
			return "A";
		case sf::Keyboard::B:
			return "B";
		case sf::Keyboard::C:
			return "C";
		case sf::Keyboard::D:
			return "D";
		case sf::Keyboard::E:
			return "E";
		case sf::Keyboard::F:
			return "F";
		case sf::Keyboard::G:
			return "G";
		case sf::Keyboard::H:
			return "H";
		case sf::Keyboard::I:
			return "I";
		case sf::Keyboard::J:
			return "J";
		case sf::Keyboard::K:
			return "K";
		case sf::Keyboard::L:
			return "L";
		case sf::Keyboard::M:
			return "M";
		case sf::Keyboard::N:
			return "N";
		case sf::Keyboard::O:
			return "O";
		case sf::Keyboard::P:
			return "P";
		case sf::Keyboard::Q:
			return "Q";
		case sf::Keyboard::R:
			return "R";
		case sf::Keyboard::S:
			return "S";
		case sf::Keyboard::T:
			return "T";
		case sf::Keyboard::U:
			return "U";
		case sf::Keyboard::V:
			return "V";
		case sf::Keyboard::W:
			return "W";
		case sf::Keyboard::X:
			return "X";
		case sf::Keyboard::Y:
			return "Y";
		case sf::Keyboard::Z:
			return "Z";
		case sf::Keyboard::Num0:
			return "0";
		case sf::Keyboard::Num1:
			return "1";
		case sf::Keyboard::Num2:
			return "2";
		case sf::Keyboard::Num3:
			return "3";
		case sf::Keyboard::Num4:
			return "4";
		case sf::Keyboard::Num5:
			return "5";
		case sf::Keyboard::Num6:
			return "6";
		case sf::Keyboard::Num7:
			return "7";
		case sf::Keyboard::Num8:
			return "8";
		case sf::Keyboard::Num9:
			return "9";
		case sf::Keyboard::Escape:
			return "Escape";
		case sf::Keyboard::LControl:
			return "LControl";
		case sf::Keyboard::LShift:
			return "LShift";
		case sf::Keyboard::LAlt:
			return "LAlt";
		case sf::Keyboard::LSystem:
			return "LSystem";
		case sf::Keyboard::RControl:
			return "RControl";
		case sf::Keyboard::RShift:
			return "RShift";
		case sf::Keyboard::RAlt:
			return "RAlt";
		case sf::Keyboard::RSystem:
			return "RSystem";
		case sf::Keyboard::Menu:
			return "Menu";
		case sf::Keyboard::LBracket:
			return "LBracket";
		case sf::Keyboard::RBracket:
			return "RBracket";
		case sf::Keyboard::SemiColon:
			return "SemiColon";
		case sf::Keyboard::Comma:
			return "Comma";
		case sf::Keyboard::Period:
			return "Period";
		case sf::Keyboard::Quote:
			return "Quote";
		case sf::Keyboard::Slash:
			return "Slash";
		case sf::Keyboard::BackSlash:
			return "BackSlash";
		case sf::Keyboard::Tilde:
			return "Tilde";
		case sf::Keyboard::Equal:
			return "Equal";
		case sf::Keyboard::Dash:
			return "Dash";
		case sf::Keyboard::Space:
			return "Space";
		case sf::Keyboard::Return:
			return "Return";
		case sf::Keyboard::BackSpace:
			return "BackSpace";
		case sf::Keyboard::Tab:
			return "Tab";
		case sf::Keyboard::PageUp:
			return "PageUp";
		case sf::Keyboard::PageDown:
			return "PageDown";
		case sf::Keyboard::End:
			return "End";
		case sf::Keyboard::Home:
			return "Home";
		case sf::Keyboard::Insert:
			return "Insert";
		case sf::Keyboard::Delete:
			return "Delete";
		case sf::Keyboard::Add:
			return "Add";
		case sf::Keyboard::Subtract:
			return "Subtract";
		case sf::Keyboard::Multiply:
			return "Multiply";
		case sf::Keyboard::Divide:
			return "Divide";
		case sf::Keyboard::Left:
			return "Left";
		case sf::Keyboard::Right:
			return "Right";
		case sf::Keyboard::Up:
			return "Up";
		case sf::Keyboard::Down:
			return "Down";
		case sf::Keyboard::Numpad0:
			return "Num0";
		case sf::Keyboard::Numpad1:
			return "Num1";
		case sf::Keyboard::Numpad2:
			return "Num2";
		case sf::Keyboard::Numpad3:
			return "Num3";
		case sf::Keyboard::Numpad4:
			return "Num4";
		case sf::Keyboard::Numpad5:
			return "Num5";
		case sf::Keyboard::Numpad6:
			return "Num6";
		case sf::Keyboard::Numpad7:
			return "Num7";
		case sf::Keyboard::Numpad8:
			return "Num8";
		case sf::Keyboard::Numpad9:
			return "Num9";
		case sf::Keyboard::F1:
			return "F1";
		case sf::Keyboard::F2:
			return "F2";
		case sf::Keyboard::F3:
			return "F3";
		case sf::Keyboard::F4:
			return "F4";
		case sf::Keyboard::F5:
			return "F5";
		case sf::Keyboard::F6:
			return "F6";
		case sf::Keyboard::F7:
			return "F7";
		case sf::Keyboard::F8:
			return "F8";
		case sf::Keyboard::F9:
			return "F9";
		case sf::Keyboard::F10:
			return "F10";
		case sf::Keyboard::F11:
			return "F11";
		case sf::Keyboard::F12:
			return "F12";
		case sf::Keyboard::F13:
			return "F13";
		case sf::Keyboard::F14:
			return "F14";
		case sf::Keyboard::F15:
			return "F15";
		case sf::Keyboard::Pause:
			return "Pause";
		}
	}
};