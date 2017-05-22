#include "FileIO/ParserTools.h"

std::vector<Condition> ParserTools::parseConditions(const std::string& toParse_, bool negativeConditions) {
	std::vector<Condition> conditions;
	std::string toParse = toParse_;

	size_t pos;

	while (!toParse.empty()) {
		if ((pos = toParse.find(",")) == std::string::npos) {
			return conditions;
		}

		std::string conditionType = toParse.substr(0, pos);
		toParse.erase(0, pos + 1);
		std::string conditionName;

		if ((pos = toParse.find(",")) != std::string::npos) {
			conditionName = toParse.substr(0, pos);
			toParse.erase(0, pos + 1);
		}
		else {
			conditionName = toParse;
			toParse.clear();
		}

		Condition condition;
		condition.type = conditionType;
		condition.name = conditionName;
		condition.negative = negativeConditions;
		conditions.push_back(condition);
	}

	return conditions;
}