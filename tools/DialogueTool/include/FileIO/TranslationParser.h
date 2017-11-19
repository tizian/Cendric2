#pragma once

#include "global.h"

struct SwissTranslation final {
	std::string tag;
	std::string swissGerman;
};

// utility for our swiss germans
class TranslationParser final {
public:
	TranslationParser();
	~TranslationParser();

	void showParseMenu();
	void doTranslationOut() const;

private:
	void doTranslationOutFor(const std::string& file) const;
};