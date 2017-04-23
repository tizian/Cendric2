#pragma once

#include "global.h"

// some constants used by configuration reader & writer
class ConfigurationIO {
public:
	virtual ~ConfigurationIO() {}
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
	const char* ALTERNATIVE_INPUT_MAPPING = "input.alternative.key";
	const char* QUICKCAST_ON = "quickcast.on";
	const char* DEBUGMODE_ON = "debugmode.on";
	const char* DEBUGRENDERING_ON = "debugrendering.on";
	const char* SMOOTHING_ON = "smoothing.on";
	const char* HINTS_ON = "hints.on";
	const char* DAMAGENUMBERS_ON = "damagenumbers.on";
};