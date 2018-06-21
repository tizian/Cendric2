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
#include "Enums/DisplayMode.h"
#include "Enums/GamepadAxis.h"
#include "GamepadInput.h"

// pure static class used to resolve enum names.
class EnumNames final {
private:
	EnumNames() = default;

	static std::string getUtf8(int i) {
		const char combined[2] = { static_cast<char>(i), '\0' };
		return std::string(combined);
	}

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
		case Language::LANG_ES:
			return "Spanish";
		}
	}

	static std::string getDisplayModeName(DisplayMode mode) {
		switch (mode) {
		default:
		case DisplayMode::MAX:
		case DisplayMode::VOID:
			return "Unknown";
		case DisplayMode::Window:
			return "Window";
		case DisplayMode::Fullscreen:
			return "Fullscreen";
		case DisplayMode::WindowedFullscreen:
			return "WindowedFullscreen";
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
		case ItemType::Spell:
			return "Spell";
		case ItemType::Key:
			return "Key";
		}
	}

	static std::string getFractionIDName(FractionID id) {
		switch (id) {
		default:
		case FractionID::MAX:
			return "Unknown";
		case FractionID::VOID:
			return "NoGuild";
		case FractionID::Cleric:
			return "cleric";
		case FractionID::Necromancer:
			return "necromancer";
		case FractionID::Thief:
			return "thief";
		}
	}

	static std::string getShortFractionIDName(FractionID id) {
		switch (id) {
		case FractionID::MAX:
		case FractionID::VOID:
			return "Unknown";
		default:
			return getFractionIDName(id) + "_short";
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
		case EnemyID::Skeleton_Default:
			return "SkeletonDefault";
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
		case EnemyID::Cairn:
			return "Cairn";
		case EnemyID::Book:
			return "Book";
		case EnemyID::Boss_Zeff:
			return "BossZeff";
		case EnemyID::Boss_Wolf:
			return "BossKhajag";
		case EnemyID::Observer:
			return "Observer";
		case EnemyID::Warden:
			return "Warden";
		case EnemyID::Prisoner:
			return "Prisoner";
		case EnemyID::Ooze:
			return "Ooze";
		case EnemyID::Bat:
			return "Bat";
		case EnemyID::Skeleton_Archer:
			return "SkeletonArcher";
		case EnemyID::Skeleton_Mage:
		case EnemyID::Skeleton_Elemental:
			return "SkeletonMage";
		case EnemyID::Skeleton_Rogue:
			return "SkeletonRogue";
		case EnemyID::Skeleton_Shield:
			return "SkeletonShield";
		case EnemyID::Skeleton_Warrior:
			return "SkeletonWarrior";
		case EnemyID::Elysia_Fledgling:
			return "ElysiaFledgling";
		case EnemyID::Elysia_Clone:
		case EnemyID::Boss_Elysia:
			return "BossElysia";
		case EnemyID::Dragonwhelp:
			return "DragonWhelp";
		case EnemyID::Boss_Janus:
			return "BossJanus";
		case EnemyID::NecroticGrimoire:
			return "NecroticGrimoire";
		case EnemyID::Boss_Morgiana:
			return "BossMorgiana";
		case EnemyID::Boss_Roy:
			return "BossRoy";
		case EnemyID::Boss_Jeremy:
			return "BossJeremy";
		case EnemyID::Leeroy:
			return "Leeroy";
		case EnemyID::Koray:
			return "Koray";
		case EnemyID::Boss_Yasha:
			return "BossYasha";
		case EnemyID::YashaAdd:
			return "YashaAdd";
		case EnemyID::Yaslaw:
			return "Yaslaw";
		case EnemyID::Boss_Royalguard_T:
		case EnemyID::Boss_Royalguard_I:
			return "BossRoyalguard";
		case EnemyID::King:
			return "King";
		case EnemyID::Boss_Velius:
		case EnemyID::VeliusClone:
			return "BossVelius";
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
		case Key::Move_Down:
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
		case Key::Move_Left:
			return "MoveLeft";
		case Key::Quickload:
			return "Quickload";
		case Key::Quicksave:
			return "Quicksave";
		case Key::Journal:
			return "Journal";
		case Key::Confirm:
			return "Confirm";
		case Key::Move_Right:
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
		case Key::NextSpell:
			return "NextSpell";
		case Key::PreviousSpell:
			return "PreviousSpell";
		case Key::Move_Up:
			return "MoveUp";
		case Key::ToggleTooltips:
			return "ToggleTooltips";
		case Key::SwitchTarget:
			return "SwitchTarget";
		case Key::Screenshot:
			return "Screenshot";
		case Key::BackToCheckpoint:
			return "BackToCheckpoint";
		case Key::ToggleAutotarget:
			return "ToggleAutotarget";
		case Key::Menu:
			return "Menu";
		case Key::Attack:
			return "Attack";
		case Key::Move_Right2:
			return "MoveRight2";
		case Key::Move_Down2:
			return "MoveDown2";
		case Key::Move_Left2:
			return "MoveLeft2";
		case Key::Move_Up2:
			return "MoveUp2";
		case Key::Aim_Right:
			return "AimRight";
		case Key::Aim_Down:
			return "AimDown";
		case Key::Aim_Left:
			return "AimLeft";
		case Key::Aim_Up:
			return "AimUp";
		}
	}

	static std::string getGamepadAxisName(GamepadAxis axis) {
		switch (axis)
		{
		default:
		case GamepadAxis::VOID:
			return "?";
		case GamepadAxis::DPadUp:
			return "DPadUp";
		case GamepadAxis::DPadDown:
			return "DPadDown";
		case GamepadAxis::DPadLeft:
			return "DPadLeft";
		case GamepadAxis::DPadRight:
			return "DPadRight";
		case GamepadAxis::LeftStickUp:
			return "LSUp";
		case GamepadAxis::LeftStickDown:
			return "LSDown";
		case GamepadAxis::LeftStickLeft:
			return "LSLeft";
		case GamepadAxis::LeftStickRight:
			return "LSRight";
		case GamepadAxis::RightStickUp:
			return "RSUp";
		case GamepadAxis::RightStickDown:
			return "RSDown";
		case GamepadAxis::RightStickLeft:
			return "RSLeft";
		case GamepadAxis::RightStickRight:
			return "RSRight";
		case GamepadAxis::LeftTrigger:
			return "LT";
		case GamepadAxis::RightTrigger:
			return "RT";
		case GamepadAxis::LeftButton1:
			return "L1";
		case GamepadAxis::RightButton1:
			return "R1";
		case GamepadAxis::LeftButton2:
			return "L2";
		case GamepadAxis::RightButton2:
			return "R2";
		case GamepadAxis::LeftButton3:
			return "L3";
		case GamepadAxis::RightButton3:
			return "R3";
		case GamepadAxis::Square:
			// ¾
			return getUtf8(0xbe);
		case GamepadAxis::Circle:
			return "O";
		case GamepadAxis::Triangle:
			// ½
			return getUtf8(0xbd);
		case GamepadAxis::A:
			return "A";
		case GamepadAxis::B:
			return "B";
		case GamepadAxis::X:
			return "X";
		case GamepadAxis::Y:
			return "Y";
		case GamepadAxis::Select:
			return "Select";
		case GamepadAxis::Start:
			return "Start";
		case GamepadAxis::Share:
			return "Share";
		case GamepadAxis::Options:
			return "Options";
		case GamepadAxis::Back:
			return "Back";
		case GamepadAxis::LogitechButton:
			return "Logitech";
		case GamepadAxis::PSButton:
			return "PS";
		case GamepadAxis::Touchpad:
			return "Touch";
		case GamepadAxis::LeftStickPush:
			return "LPush";
		case GamepadAxis::RightStickPush:
			return "RPush";
		case GamepadAxis::LeftShoulder:
			return "LB";
		case GamepadAxis::RightShoulder:
			return "RB";
		}
	}

	static std::string getGamepadInputName(GamepadInput input) {
		switch (input)
		{
		case GamepadInput::VOID: 
		default:
			return "?";
		case GamepadInput::Axis_X_Positive: 
			return "X+";
		case GamepadInput::Axis_X_Negative: 
			return "X-";
		case GamepadInput::Axis_Y_Positive: 
			return "Y+";
		case GamepadInput::Axis_Y_Negative: 
			return "Y-";
		case GamepadInput::Axis_Z_Positive: 
			return "Z+";
		case GamepadInput::Axis_Z_Negative: 
			return "Z-";
		case GamepadInput::Axis_R_Positive: 
			return "R+";
		case GamepadInput::Axis_R_Negative: 
			return "R-";
		case GamepadInput::Axis_U_Positive: 
			return "U+";
		case GamepadInput::Axis_U_Negative: 
			return "U-";
		case GamepadInput::Axis_V_Positive: 
			return "V+";
		case GamepadInput::Axis_V_Negative: 
			return "V-";
		case GamepadInput::Axis_PovX_Positive: 
			return "PX+";
		case GamepadInput::Axis_PovX_Negative: 
			return "PX-";
		case GamepadInput::Axis_PovY_Positive: 
			return "PY+";
		case GamepadInput::Axis_PovY_Negative: 
			return "PX-";
		case GamepadInput::Button_0: 
			return "B0";
		case GamepadInput::Button_1: 
			return "B1";
		case GamepadInput::Button_2: 
			return "B2";
		case GamepadInput::Button_3: 
			return "B3";
		case GamepadInput::Button_4: 
			return "B4";
		case GamepadInput::Button_5: 
			return "B5";
		case GamepadInput::Button_6:
			return "B6";
		case GamepadInput::Button_7:
			return "B7";
		case GamepadInput::Button_8:
			return "B8";
		case GamepadInput::Button_9:
			return "B9";;
		case GamepadInput::Button_10:  
			return "B10";
		case GamepadInput::Button_11:  
			return "B11";
		case GamepadInput::Button_12:  
			return "B12";
		case GamepadInput::Button_13:  
			return "B13";
		case GamepadInput::Button_14:  
			return "B14";
		case GamepadInput::Button_15:  
			return "B15";
		case GamepadInput::Button_16:  
			return "B16";
		case GamepadInput::Button_17:  
			return "B17";
		case GamepadInput::Button_18:  
			return "B18";
		case GamepadInput::Button_19:  
			return "B19";
		case GamepadInput::Button_20:  
			return "B20";
		case GamepadInput::Button_21:  
			return "B21";
		case GamepadInput::Button_22:  
			return "B22";
		case GamepadInput::Button_23:  
			return "B23";
		case GamepadInput::Button_24:  
			return "B24";
		case GamepadInput::Button_25:  
			return "B25";
		case GamepadInput::Button_26:  
			return "B26";
		case GamepadInput::Button_27:  
			return "B27";
		case GamepadInput::Button_28:  
			return "B28";
		case GamepadInput::Button_29:  
			return "B29";
		case GamepadInput::Button_30:  
			return "B30";
		case GamepadInput::Button_31:  
			return "B31";
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

	static std::string getShortKeyboardKeyName(sf::Keyboard::Key key) {
		switch (key) {
		case sf::Keyboard::Unknown:
		case sf::Keyboard::KeyCount:
			return "??";
		case sf::Keyboard::Escape:
			return "Esc";
		case sf::Keyboard::LControl:
			return "LCt";
		case sf::Keyboard::LShift:
			return "LSh";
		case sf::Keyboard::LAlt:
			return "LAl";
		case sf::Keyboard::LSystem:
			return "LSy";
		case sf::Keyboard::RControl:
			return "RCt";
		case sf::Keyboard::RShift:
			return "RSh";
		case sf::Keyboard::RAlt:
			return "RAl";
		case sf::Keyboard::RSystem:
			return "RSy";
		case sf::Keyboard::Menu:
			return "Men";
		case sf::Keyboard::LBracket:
			return "LBr";
		case sf::Keyboard::RBracket:
			return "RBr";
		case sf::Keyboard::SemiColon:
			return ";";
		case sf::Keyboard::Comma:
			return ",";
		case sf::Keyboard::Period:
			return ".";
		case sf::Keyboard::Quote:
			return "\"";
		case sf::Keyboard::Slash:
			return "/";
		case sf::Keyboard::BackSlash:
			return "\\";
		case sf::Keyboard::Tilde:
			return "~";
		case sf::Keyboard::Equal:
			return "=";
		case sf::Keyboard::Dash:
			return "-";
		case sf::Keyboard::Space:
			return "Spa";
		case sf::Keyboard::Return:
			return "Ret";
		case sf::Keyboard::BackSpace:
			return "<--";
		case sf::Keyboard::PageUp:
			return "PgU";
		case sf::Keyboard::PageDown:
			return "PgD";
		case sf::Keyboard::Home:
			return "Hom";
		case sf::Keyboard::Insert:
			return "In";
		case sf::Keyboard::Delete:
			return "Del";
		case sf::Keyboard::Add:
			return "Add";
		case sf::Keyboard::Subtract:
			return "Sub";
		case sf::Keyboard::Multiply:
			return "Mul";
		case sf::Keyboard::Divide:
			return "Div";
		case sf::Keyboard::Left:
			return "<-";
		case sf::Keyboard::Right:
			return "->";
		case sf::Keyboard::Up:
			return "Up";
		case sf::Keyboard::Down:
			return "Dwn";
		case sf::Keyboard::Numpad0:
			return "Nu0";
		case sf::Keyboard::Numpad1:
			return "Nu1";
		case sf::Keyboard::Numpad2:
			return "Nu2";
		case sf::Keyboard::Numpad3:
			return "Nu3";
		case sf::Keyboard::Numpad4:
			return "Nu4";
		case sf::Keyboard::Numpad5:
			return "Nu5";
		case sf::Keyboard::Numpad6:
			return "Nu6";
		case sf::Keyboard::Numpad7:
			return "Nu7";
		case sf::Keyboard::Numpad8:
			return "Nu8";
		case sf::Keyboard::Numpad9:
			return "Nu9";
		case sf::Keyboard::Pause:
			return "Pau";
		default:
			break;
		}

		return getKeyboardKeyName(key);
	}

	static std::string getArrowSymbolForKey(Key key)
	{
		switch (key)
		{
		default:
			return "?";

		case Key::Move_Up:
			// ¹
			return getUtf8(0xb9);
		
		case Key::Move_Down:
			// º
			return getUtf8(0xba);

		case Key::Move_Left:
			// »
			return getUtf8(0xbb);

		case Key::Move_Right:
			// ¼
			return getUtf8(0xbc);
		}
	}
};