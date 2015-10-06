#pragma once

#include "global.h"

// some constants used by configuration reader & writer
class ConfigurationIO {
protected:
	const char* LANGUAGE = "language";
	const char* MAX_FPS = "fps.max";
	const char* SOUND_ON = "sound.on";
	const char* SOUND_VOLUME = "sound.volume";
	const char* MAIN_INPUT_MAPPING = "input.main.key";
	const char* ALTERNATIVE_INPUT_MAPPING = "input.alternative.key";
	const char* QUICKCAST_ON = "quickcast.on";
	const char* DEBUGMODE_ON = "debugmode.on";
	const char* DEBUGRENDERING_ON = "debugrendering.on";
	const char* FULLSCREEN_ON = "fullscreen.on";
	const char* SMOOTHING_ON = "smoothing.on";
};