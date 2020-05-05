#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>


#include<termios.h>

#include "ped.h"
#include "commands.c"

#include<dlfcn.h>
#include "tty.c"

int main(int argc, char** args)
{
	/* Make sure we have only two arguments */
	if(argc == 2)
	{
		/* Get the filename */
		char* filename = *(args+1);

		/* Create a new session */
		struct Session* session = newSession(filename);

		if(session)
		{
			/* Open the editor */
			newEditor(session);

			/* Free the session */
			free(session);
		}

		
	}
	else
	{
		printf("Too many args, or missing a file.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void output(char* string, unsigned int length)
{
	write(0, string, length);
}

char getChar()
{
	char character;
	int b = read(0, &character, 1);
	return character;
}

void redraw(struct Session* session)
{
	/* Move cursor back home */
	char cr = 13;
	output(&cr, 1);

	/* Print all the data */
	output(session->data, session->size);

	/* Move cursor back home */
	output(&cr, 1);

	unsigned int i = 0;
	while(i < session->position)
	{
		char seq[3] = {27, 91, 67};
		output(seq, 3);
		i++;
	}
}

void newEditor(struct Session* session)
{
	/* Setup the tty */
	startTTY();

	/* Output the file as of now */
	redraw(session);

	//output("hello world", strlen("hello world"));
	while(session->isActive)
	{
		char s;
		s=getChar();

		if (s == 17)
		{
			session->isActive = 0;
		}
		/* Control */
		else if(s == 27)
		{
			/* Arrow keys */
			s=getChar();
			if(s == 91)
			{
				s=getChar();
				char seq[3];
				/* Up arrow */
				if(s == 65)
				{
				
				}
				/* Down arrow */
				else if (s == 66)
				{
					
				}
				/* Right arrow */
				else if(s == 67)
				{
					seq[0] = 27;
					seq[1] = 91;
					seq[2] = 67;

					session->position++;
				}
				/* Left arrow */
				else if (s == 68)
				{
					seq[0] = 27;
					seq[1] = 91;
					seq[2] = 68;

					/* Only move cursor back if we not at home */
					if(session->position)
					{
						session->position--;
					}
					
				}

			//	output(seq, 3);
			//continue;
				
			}
		}
		/* Ctrl+D is command key */
		else if(s == 4)
		{
			char* str = malloc(200);
			*str = 0;

			while(1)
			{
				s=getChar();

				if(s==4)
				{
					break;
				}
				else
				{
					strncat(str, &s, 1);
				}
			}
			output(str,strlen(str));

			runCommand(str, session);
			
			
			free(str);
		}
		else
		{
			/* Set data at current position */
			*(session->data+session->position) = s;
			/* TODO: As we type position increases */
			session->position++;
			//strncat(session->data, &s, 1);
			session->size++;
		}
		
		//printf("%c\n",s);
		char* l = malloc(20);
		sprintf(l, "%u", s);
	//	output(l, strlen(l));
	//	output(&s, 1);
		
		/* Redraw */
		redraw(session);		
	}

	char* bye = "\nBye mate!\n";
	output(bye, strlen(bye));
	
	/* Restore tty settings */
	stopTTY();
}

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
			

			/* Set initial position to 0 */
			session->position = session->size;

			/* Set the session to active */
			session->isActive = 1;

			/* On success, return the pointer to the session */
			return session;
		}

		/* On successful malloc, but file failure */
		free(session);
	}

	/* On error */
	return 0;
}
