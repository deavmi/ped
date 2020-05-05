#include <string.h>
#include "types.h"
#include "ped.h"
#include<dlfcn.h>
#include<stdlib.h>

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
	else if(strcmp(str, "w") == 0)
	{
		write(session->fd, session->data, session->size);
	}
	/* TODO: Replace with file mapping */
	/* This tries to load the object file provided */
	else
	{
		/* Generate the object file name */
		char* objectFile = malloc(strlen(str)+3);
		strcpy(objectFile, str);
		strcat(objectFile, ".o");
		
		void* dynObjHandle = dlopen(objectFile, RTLD_NOW);

		if(dynObjHandle)
		{
			void (*funcPtr)(struct Session*) = dlsym(dynObjHandle, "dispatch");

			if(funcPtr)
			{
				funcPtr(session);
			}
			else
			{
				output("Error finding symbol", strlen("Error finding SYMBOL"));
			}
		}
		else
		{
			output("Error loading object file", strlen("Error loading object file"));
		}
	}
}
