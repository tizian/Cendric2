#include "DialogueTool.h"
#include "ApplicationState.h"
 
int main()
{
	g_state = new ApplicationState();
	g_state->load();
	DialogueTool* tool = new DialogueTool();
	tool->run();
	delete tool;
	delete g_state;

	return 0;
}