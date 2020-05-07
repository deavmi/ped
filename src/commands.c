#include <string.h>
#include "types.h"
#include "ped.h"
#include<dlfcn.h>
#include<stdlib.h>
#include<unistd.h>

char** getCommand(char* str)
{
	unsigned int i = 0;
	char* newThing = malloc(1);

	char active = 1;
	while(active)
	{
		/* Get the current byte */
		char currentByte = *(str+i);
		
		if(currentByte == ' ' || currentByte == 0)
		{
			*(newThing+i) = 0;
			active=0;
			i++;
			continue;
		}
		else
		{
			*(newThing+i) = currentByte;
		}

		i++;
		newThing = realloc(newThing, i+1);
	}

	char* args = str+i;

	char** stuff = malloc(sizeof(char*)*2);
	*stuff=newThing;
	*(stuff+1)=args;

	return stuff;
}


void runCommand(char* str, struct Editor* editor)
{
	/* TODO: Split the string by first space */
	/* TODO: First item is command, after space is args */

	

	/* Get command and arguments */
	char* command = *getCommand(str);
	output(command, strlen(command));

	/* Get arguments */
	char* args = *(getCommand(str)+1);
	output(args, strlen(args));

	if(strcmp(command, "help") == 0)
	{
		output("NO HELP!", strlen("NO HELP!"));
	}
	else if(strcmp(command, "quit") == 0)
	{
		editor->currentSession->isActive = 0;
	}
	else if(strcmp(command, "w") == 0)
	{
		write(editor->currentSession->fd, editor->currentSession->data, editor->currentSession->size);
	}
	/* TODO: Replace with file mapping */
	/* This tries to load the object file provided */
	else
	{
		//return;
		/* Generate the object file name */
		char* objectFile = malloc(strlen(str)+3);
		strcpy(objectFile, command);
		strcat(objectFile, ".o");
		
		void* dynObjHandle = dlopen(objectFile, RTLD_NOW);

		if(dynObjHandle)
		{
			unsigned char (*funcPtr)(struct Editor*, char*, void*) = dlsym(dynObjHandle, "dispatch");

			if(funcPtr)
			{
				funcPtr(editor, "onCommand", NULL);
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
