#pragma once

#include "global.h"
#include "GlobalResource.h"

enum class GUIOrnamentStyle {
	NONE,
	SMALL,
	MEDIUM,
	LARGE
};

inline sf::Texture *getOrnamentStyleTexture(GUIOrnamentStyle style) {
	if (style == GUIOrnamentStyle::SMALL) {
		return g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_SMALL);
	}
	else if (style == GUIOrnamentStyle::MEDIUM) {
		return g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_MEDIUM);
	}
	else if (style == GUIOrnamentStyle::LARGE) {
		return g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_LARGE);
	}
	else {
		return g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE);
	}
}