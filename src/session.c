#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>

#include "types.h"

/**
* Allocates a new struct Session on the heap
* and associates a new file with it and all.
*/
struct Session* newSession(char* filename)
{
	/* Allocate the session */
	struct Session* session = malloc(sizeof(struct Session));

	/* Make sure it allocated */
	if(session)
	{
		/* Open the file */
		int fd = open(filename, O_CREAT|O_RDWR);

		if(fd >= 0)
		{
			/* Set the file descriptor */
			session->fd = fd;

			/* Set name */
			session->name = filename;

			/* Get the stat struct */
			struct stat statStruct;

			/* Fill stat struct */
			fstat(fd, &statStruct);
	
			/* Set the initial size */
			session->size = statStruct.st_size;

			/* Allocate initial space for buffer */
			/* TODO: Remove and be dynamic */
			session->data = malloc(session->size+69);

			/* Set the initial characters */
			read(fd, session->data, session->size);

			char* temp = malloc(session->size+1);
			strncpy(temp, session->data, session->size);
			//printf("%s", temp);
			

			/* TODO: Use xy :: Set initial position to 0 */
			//session->fileX = session->size;
			session->fileX = 0;
			session->fileY = 0;
			//session->fileY = linefeedCount(session->data, session->size);
			/* Set the tty */
			session->teletype = newTTY();
			//session->teletype->cursorX = session->fileX;
			
			
			/* Set the session to active */
			session->isActive = 1;

			/* Set the plugins to 0 */
			

			/* On success, return the pointer to the session */
			return session;
		}

		/* On successful malloc, but file failure */
		free(session);
	}

	/* On error */
	return 0;
}
