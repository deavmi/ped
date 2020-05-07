#include "../types.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/**
* Sessions
*
* Session management by Tristan and Stephen.
*/
unsigned char dispatch(struct Editor* editor, char* callType, void* args)
{
	/* Check for what we are being called */
	if(strcmp(callType, "onCommand") == 0)
	{
		/* Get arguments */
		char** actualArgs = (char**)args;

		/* TODO: Continue here */
		return 0;
	}

	return 1;
}