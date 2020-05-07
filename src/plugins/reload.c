#include "../types.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

/**
* Reload
*
* Closes the currently open fd, reopens it and
* reads from it into the data buffer.
*/

unsigned char dispatch(struct Editor* editor, char* callType, void* args)
{
	if(strcmp(callType, "onCommand") == 0)
	{
		/* Close the current file */
		close(editor->currentSession->fd);

		/* TODO: Error check */

		/* Reopen */
		editor->currentSession->fd = open(editor->currentSession->name, O_RDWR);

		/* Get the stat struct */
		struct stat statStruct;

		/* Fill stat struct */
		fstat(editor->currentSession->fd, &statStruct);
	
		/* Set the file size */
		editor->currentSession->size = statStruct.st_size;

		/* Free the old file data */
		free(editor->currentSession->data);

		/* Allocate new data */
		editor->currentSession->data = malloc(editor->currentSession->size);

		/* Set the initial characters */
		read(editor->currentSession->fd, editor->currentSession->data, editor->currentSession->size);
	}
	
}