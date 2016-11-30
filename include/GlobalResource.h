#pragma once

#include <iostream>

// a static class to store the global resource paths
// to be included in .cpp files, not headers, thanks.
class GlobalResource {
public:
	static const std::string SAVEGAME_FOLDER;
	static const std::string CONFIGURATION_PATH;
	static const std::string QUICKSAVE_PATH;
	static const std::string AUTOSAVE_PATH;
	static const std::string NPC_FOLDER;
	static const std::string QUEST_FOLDER;
	static const std::string SCREENSHOT_FOLDER;

	static const std::string FONT_8;
	static const std::string FONT_8_SHADOWED;
	static const std::string FONT_12;
	static const std::string FONT_12_SHADOWED;

	static const std::string TEX_PARTICLE_CIRCLE;
	static const std::string TEX_PARTICLE_BLOB;
	static const std::string TEX_PARTICLE_LONGBLOB;
	static const std::string TEX_PARTICLE_BEAM;
	static const std::string TEX_PARTICLE_LIGHT;
	static const std::string TEX_PARTICLE_SNOWFLAKE;
	static const std::string TEX_PARTICLE_SMOKE;
	static const std::string TEX_PARTICLE_STAR;
	static const std::string TEX_PARTICLE_RAIN;
	static const std::string TEX_PARTICLE_SNOW;
	static const std::string TEX_PARTICLE_FLAME;
	static const std::string TEX_PARTICLE_DROP;
	static const std::string TEX_PARTICLE_FIRE;
	static const std::string TEX_PARTICLE_CONFETTI;

	static const std::string TEX_ITEMS;
	static const std::string TEX_LEVELITEMS;
	static const std::string TEX_SPELLICONS;
	static const std::string TEX_SPELLSCROLL;
	static const std::string TEX_INVENTORYTABS;
	static const std::string TEX_SPELLBOOKTABS;
	static const std::string TEX_GUILDICONS;
	static const std::string TEX_EQUIPMENTPLACEHOLDERS;
	static const std::string TEX_GEMS;
	
	// map resources
	static const std::string TEX_MAPMARKERS;
	static const std::string TEX_DIALOGUE;
	static const std::string TEX_COOKING;
	static const std::string SOUND_TELEPORT;

	// level resources
	static const std::string TEX_DAMAGETYPES;
	static const std::string TEX_DEBUFF_FEAR;
	static const std::string TEX_DEBUFF_STUN;
	static const std::string TEX_SCREEN_GAMEOVER;
	static const std::string TEX_SCREEN_OVERLAY;
	static const std::string TEX_SCREEN_OVERLAY_STUNNED;
	static const std::string TEX_SCREEN_OVERLAY_FEARED;
	static const std::string TEX_SCREEN_OVERLAY_DAMAGED;
	static const std::string TEX_TEXT_GAMEOVER;
	static const std::string TEX_TEXT_GAMEPAUSED;
	static const std::string TEX_TEXT_DEFEATED;
	static const std::string TEX_TEXT_ARRESTED;
	// level resources (gui)
	static const std::string TEX_GUI_SPEECHBUBBLE_POINTER;
	static const std::string TEX_GUI_HEALTHBAR_MAINCHAR_BORDER;
	static const std::string TEX_GUI_HEALTHBAR_ENEMY_BORDER;
	static const std::string TEX_GUI_HEALTHBAR_BOSS_BORDER;
	static const std::string TEX_GUI_HEALTHBAR_CONTENT;
	static const std::string TEX_GUI_HEALTHBAR_CONTENT_HIT;
	static const std::string TEX_GUI_HEALTHBAR_CONTENT_HIGHLIGHT;

	// gui resources (global)
	static const std::string TEX_GUI_ROUNDED_RECTANGLE;
	static const std::string TEX_GUI_ORNAMENT_NONE;
	static const std::string TEX_GUI_ORNAMENT_SMALL;
	static const std::string TEX_GUI_ORNAMENT_MEDIUM;
	static const std::string TEX_GUI_ORNAMENT_LARGE;
	static const std::string TEX_GUI_WINDOW_CLOSE_BUTTON;
	static const std::string TEX_GUI_ARROW;
	static const std::string TEX_GUI_ARROW_LEFT;
	static const std::string TEX_GUI_ARROW_RIGHT;
	static const std::string TEX_GUI_EXIT_ARROW;
	static const std::string TEX_GUI_LADDER_ARROW;
	static const std::string TEX_GUI_CHECKBOX;
	static const std::string TEX_GUI_SLIDER;
	static const std::string TEX_GUI_KNOB;
	static const std::string TEX_GUI_SLOT_INVENTORY;
	static const std::string TEX_GUI_SLOT_INVENTORY_SELECTED;
	static const std::string TEX_GUI_SLOT_MODIFIER;
	static const std::string TEX_GUI_SLOT_MODIFIER_SELECTED;
	static const std::string TEX_GUI_SLOT_SPELL;
	static const std::string TEX_GUI_SLOT_SPELL_SELECTED;
	static const std::string TEX_GUI_SLOT_SPELL_GEM;
	static const std::string TEX_GUI_SLOT_HIGHLIGHT;
	static const std::string TEX_GUI_SLOT_PROGRESS_LOG;
	static const std::string TEX_GUI_TAB_INACTIVE;
	static const std::string TEX_GUI_TAB_ACTIVE;
	static const std::string TEX_GUI_CURSOR;
	static const std::string TEX_GUI_PROGRESSLOG_ICONS;
	static const std::string TEX_GUI_CHARACTERINFO_ICONS;
	static const std::string TEX_GUI_STATS_ICONS;
	static const std::string TEX_GUI_BUTTON_ROUND;
	static const std::string TEX_GUI_BUTTON_ROUND_SELECTED;
	static const std::string TEX_GUI_TAB_ICONS;
	static const std::string TEX_GUI_TAB_ICONS_SELECTED;

	// sound
	static const std::string SOUND_GUI_MENUCURSOR;
	static const std::string SOUND_GUI_PICKUP;
	static const std::string SOUND_GUI_OPENWINDOW;
	static const std::string SOUND_GUI_QUESTPROGRESS;

	static const std::string SOUND_MISC_UNLOCK;
};
