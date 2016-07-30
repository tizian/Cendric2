#pragma once

#include "global.h"

class NodeCondition;

// a window that lets you build a condition
class ConditionWindow {

public:
	ConditionWindow() {}
	
	static void showWindow(NodeCondition* condition);
};