#include "../types.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/**
* Zifty
*
* Tristan's status line generator,
* is default as Tristan knows best.
*/

unsigned char dispatch(struct Editor* editor, char* callType, void* args)
{
	if(strcmp(callType, "onRedraw") == 0)
	{
		/* Get dimensions */
		unsigned int columns = editor->currentSession->teletype->columns;

		/* General counter */
		unsigned int i = 0;

		/* STatus line */
		editor->currentSession->status = malloc(columns);
		*editor->currentSession->status=0;

		/* Add name */
		strcat(editor->currentSession->status, "{🐻}️️[");
		strcat(editor->currentSession->status, editor->currentSession->name);
		strcat(editor->currentSession->status, "]");

		strcat(editor->currentSession->status, "(");
		char* temp = malloc(20);
		sprintf(temp, "%u", editor->currentSession->size);
		strcat(editor->currentSession->status, temp);
		strcat(editor->currentSession->status, ")");
		
		i = 0;
		while(i < columns-strlen(editor->currentSession->name)-2-strlen(temp)-2-2-2)
		{
			strcat(editor->currentSession->status, "-");
			i++;
		}

		//free(temp);	
	}
	
}