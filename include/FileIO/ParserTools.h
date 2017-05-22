#pragma once

#include "global.h"
#include "Structs/Condition.h"

// pure static class for some convenient parser tools
class ParserTools final {
private:
	ParserTools() {}
public:
	static std::vector<Condition> parseConditions(const std::string& toParse, bool negativeConditions);
};