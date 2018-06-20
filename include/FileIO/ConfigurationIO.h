#pragma once

// some constants used by configuration reader & writer
class ConfigurationIO {
public:
	virtual ~ConfigurationIO() = default;
protected:
	const char* LANGUAGE = "language";
	const char* DISPLAYMODE = "displaymode";
	const char* VSYNC_ON = "vsync.on";
	const char* FPS_LIMIT_ON = "fps.limit.on";
	const char* FPS_MAX = "fps.max";
	const char* SOUND_ON = "sound.on";
	const char* SOUND_VOLUME_MUSIC = "sound.volume.music";
	const char* SOUND_VOLUME_SOUND = "sound.volume.sound";
	const char* MAIN_INPUT_MAPPING = "input.main.key";
	const char* GAMEPAD_INPUT_MAPPING = "input.main.gamepad";
	const char* GAMEPAD_PRODUCT_ID = "gamepad.product.id";
	const char* QUICKCAST_ON = "quickcast.on";
	const char* AUTOTARGET_ON = "autotarget.on";
	const char* MULTITHREADING_ON = "multithreading.on";
	const char* DEBUGRENDERING_ON = "debugrendering.on";
	const char* SMOOTHING_ON = "smoothing.on";
	const char* HINTS_ON = "hints.on";
	const char* QUESTMARKERS_ON = "questmarkers.on";
	const char* PAUSEINVENTORY_ON = "pauseinventory.on";
	const char* LOG_LEVEL = "log.level";
	const char* DISPLAY_TIME = "display.time";
};