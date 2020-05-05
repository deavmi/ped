#include <string.h>
#include "types.h"
#include "ped.h"

void runCommand(char* str, struct Session* session)
{
	if(strcmp(str, "help") == 0)
	{
		output("NO HELP!", strlen("NO HELP!"));
	}
	else if(strcmp(str, "quit") == 0)
	{
		session->isActive = 0;
	}
}
