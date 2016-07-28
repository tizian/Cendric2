#include "DialogueTool.h"
 
int main()
{
	DialogueTool* tool = new DialogueTool();
	tool->run();
	delete tool;

	return 0;
}