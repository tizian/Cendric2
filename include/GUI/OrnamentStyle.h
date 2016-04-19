#pragma once

#include "global.h"

enum class GUIOrnamentStyle {
	NONE,
	SMALL,
	MEDIUM,
	LARGE
};

inline sf::Texture *getOrnamentStyleTexture(GUIOrnamentStyle style) {
	if (style == GUIOrnamentStyle::SMALL) {
		return g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_small);
	}
	else if (style == GUIOrnamentStyle::MEDIUM) {
		return g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_medium);
	}
	else if (style == GUIOrnamentStyle::LARGE) {
		return g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large);
	}
	else {
		return g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none);
	}
}