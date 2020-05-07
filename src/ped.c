#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#include "ped.h"
#include "types.h"

#include<dlfcn.h>

#include<string.h>

/* TODO: Store session here so we can swap out sessions */

/**
* Represents this editor
*/
struct Editor editor;

int main(int argc, char** args)
{
	/* Create a new editor instance */
	newEditor2(&editor);
	loadPlugins(&editor);

	/* Make sure we have only two arguments */
	if(argc == 2)
	{
		/* Get the filename */
		char* filename = *(args+1);

		/* Create a new session */
		struct Session* session = newSession(filename);

		/* TODO: Skippy make function to add it to linked list */

		/* Set the current session to it */
		setCurrentSession(&editor, session);

		if(session)
		{
			/* Open the editor */
			newEditor();

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

void ttyPut(struct Session* session, char byte)
{
	output(&byte, 1);

	/* Check if the byte if not a line feed */
	if(byte != 10)
	{
		session->teletype->cursorX++;	
	}
	/* If the byte is a linefeed */
	else if(byte == 10)
	{
		session->teletype->cursorY++;
	}
	else if(byte == 13)
	{
		session->teletype->cursorX = 0;
	}
	
}

/**
* Moves us back to the beginning of the terminal
*/
void goHome(struct Session* session)
{
	/* Move up */
	while(session->teletype->cursorY)
	{
		char seq[3] = {27, 91, 65};
		output(seq, 3);
		session->teletype->cursorY--;
	}

	/* tty put carriage return */
	ttyPut(session, 13);
}

/**
* Moves the tty cursor to session->fileX and session->fileY
*/
void moveToFilePos(struct Session* session)
{
	unsigned int i = 0;

	/* Move editor cursor to x-offset */
	i = 0;
	while(i < session->fileX)
	{
		session->teletype->cursorX++;
		char seq[3] = {27,91,67};
		output(seq, 3);
		i++;
	}

	/* Move editor cursor to y-offset */
	i = 0;
	while(i < session->fileY)
	{
		char seq[3] = {27,91,66};
		output(seq, 3);
		session->teletype->cursorY++;
		i++;
	}

}

/**
* Draws the header line
*/
void statusDraw(struct Session* session)
{
	/* General counter */
	unsigned int i = 0;

	/* This moves y and x back to 0,0 back up */
	goHome(session);

	/* Get the length of the status line */
	unsigned int statusLength = strlen(session->status);
	char* statusLine = session->status;

	while(i< statusLength)
	{
		char byte = *(statusLine+i);
		ttyPut(session, byte);
		i++;
	}
	/* Move us down */
	ttyPut(session, 10);

	/* Slam that carriage back */
	ttyPut(session, 13);
}

void runDrawPlugins()
{
	/* TODO: This is to load zifty, this should be a loop form a file */
	void* dynObjHandle = dlopen("zifty.o", RTLD_NOW);
	unsigned char (*funcPtr)(struct Editor*, char*, void*) = dlsym(dynObjHandle, "dispatch");
	funcPtr(&editor, "onRedraw", NULL);
}

void redraw(struct Session* session)
{
	/* Run all plugins that run on draw */
	runDrawPlugins();

	/* Draw the status line */
	statusDraw(session);
//	free(session->status);
	
	/* Move cursor back home */
	char cr = 13;

	/* Write all the buffer data */
	unsigned int i = 0;
	while(i < session->size)
	{
		char byte = *(session->data+i);

		if(byte == 10)
		{
			output("\n",1);
			output("\r",1);
			session->teletype->cursorX=0;
			session->teletype->cursorY++;
		}
		else
		{
				ttyPut(session, *(session->data+i));	
		}

		
		i++;
	}

	/* First we go home and down a line */
	goHome(session);
	ttyPut(session, 10);

	/* Applies the file cursor position to the tty */
	moveToFilePos(session);
}

void initialDisplay(struct Session* session)
{
	unsigned int i = 0;
	while(i < session->size)
	{
		char byte = *(session->data+i);

		/* If the file contains a new line */
		if(byte == 10)
		{
			/**
			* If the file contains a new line
			* then we move down and back.
			*
			* And increment cursor for file.
			*/
		//	ttyPut(session, 10);
		//	ttyPut(session, 13);
			session->fileY++;
			session->fileX = 0;
		}
		else
		{
		//	ttyPut(session, byte);
			session->fileX++;
		}
		
		i++;
	}
}

unsigned int map(struct Session* session)
{
	char* dataBuffer = session->data;
	unsigned int dataBufferLength = session->size;

	unsigned int lineFeeds = session->fileY;
	unsigned int lfOccs = lineFeeds;
	
	unsigned int i = 0;
	while(i < session->size && lfOccs)
	{
		char byte = *(session->data+i);

		if(byte == 10)
		{
			lfOccs--;
		}
		i++;
	}

	return i;
}

void ring()
{
	char bell = '\a';
	output(&bell, 1);
}

void newEditor()
{
	/* The current session */
	struct Session* session = editor.currentSession;

	/* Setup the tty */
	startTTY();

	initialDisplay(session);

	/* Output the file as of now */
	redraw(session);

	

	/* Update the tty's dimensions */
	updateDimensions(session->teletype);

	/* Debugging */
	//debugTTY(open("/dev/pts/18",O_RDWR), session->teletype);

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

				/* Up arrow */
				if(s == 65)
				{
					if(session->fileY)
					{
						session->fileY--;
					}
				}
				/* Down arrow */
				else if (s == 66)
				{
					if(session->teletype->rows-2 == session->fileY)
					{
						ring();
					}
					else
					{
						session->fileY++;
					}
				}
				/* Right arrow */
				else if(s == 67)
				{
					if(session->teletype->columns-1 == session->fileX)
					{
						ring();
					}
					else
					{
						session->fileX++;
					}
				}
				/* Left arrow */
				else if (s == 68)
				{
					/* Only move cursor back if we not at home */
					if(session->fileX)
					{
						session->fileX--;
					}
					
				}

			//	output(seq, 3);
			//continue;
				
			}
		}
		/* Ctrl+D is command key */
		else if(s == 4)
		{
			/* Allocate a byte */
			char* str = malloc(1);

			/* Receive input till next Ctrl+D */
			unsigned int i = 0;
			while(1)
			{
				/* Get next character */
				s=getChar();

				if(s==4)
				{
					/* Null terminate the string */
					*(str+i) = 0;
					break;
				}
				else
				{
					/* Append new character */
					*(str+i) = s;
				}
				i++;

				str = realloc(str, i+1);
			}
			//output(str,strlen(str));

			if(strlen(str) > 0)
			{
				runCommand(str, &editor);	
			}
			if(str==0)
			{
				/* TODO: Bug here, this if statement never runs and is useless */
				free(str);	
			}
			
		}
		else
		{
			/* `newLineAfterOffset` is the offset to the character after `\n` */
			unsigned int newLineAfterOffset = map(session);

			/* `finalOffset` is the new line offset with file cursor */
			unsigned int finalOffset = newLineAfterOffset+session->fileX;

			/**
			* If the final linear address is greater-then
			* or equal to the current size we must then
			* expand the buffer.
			*/
			if(finalOffset >= session->size)
			{
				/* Increase the size by fFInal-session->size */
				session->size = finalOffset+session->fileX;
			}
			else
			{
				
			}
			
			
			/* Set data at current position */
			*(session->data+finalOffset) = s;

			/* TODO: As we type position increases */
			session->fileX++;
			//session->teletype->cursorX++;
			//strncat(session->data, &s, 1);
			
		}
		
		//printf("%c\n",s);
	//	char* l = malloc(20);
	//	sprintf(l, "%u", s);
	//	output(l, strlen(l));
	//	output(&s, 1);

		/* Update the tty's dimensions */
		updateDimensions(session->teletype);

		//debugTTY(open("/dev/pts/18",O_RDWR), session->teletype);
		
		/* Redraw */
		redraw(session);		
	}

	//char* bye = "\nBye mate!\n";
	//output(bye, strlen(bye));

	/* Clean up commands */
	
	/* Restore tty settings */
	stopTTY();

}

struct TTY* newTTY()
{
	struct TTY* tty = malloc(sizeof(struct TTY));

	updateDimensions(tty);

	return tty;
}