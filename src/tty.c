#include<termios.h>
#include<sys/ioctl.h>
#include "types.h"
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
struct termios oldSettings;

void startTTY()
{
	/* First create a struct */
	struct termios termy;

	/* Get current attributes */
	tcgetattr(0, &termy);

	/* Save current tty settings for restoral later */
	oldSettings = termy;

	/* Set the tty input modes */
	//tcflag_t inputFlags = 0;
	termy.c_iflag = termy.c_iflag | IGNBRK;
	termy.c_lflag = termy.c_lflag & ~ISIG;
	termy.c_lflag = termy.c_lflag & ~ECHO;

	cfmakeraw(&termy);
	
	/* Set the tty to raw mode */
	tcsetattr(0, 0, &termy);
}

void updateDimensions(struct TTY* tty)
{
	/* window size struct */
	struct winsize size;
		
	/* IOCTL the vnode via the fd that points to it */
	ioctl(0, TIOCGWINSZ, &size);

	/* Set the TTY's to be the same as the actual TTY's */
	tty->rows = size.ws_row;
	tty->columns = size.ws_col;

	
}

void debugTTY(int fd, struct TTY* tty)
{
	FILE* f = fdopen(fd, "w+");
	fprintf(f, "Row: %u\n", tty->rows);
	
	fprintf(f, "Column: %u\n", tty->columns);
	fprintf(f,"XPos: %u\n", tty->cursorX);
	fprintf(f,"YPos: %u\n", tty->cursorY);
}

void stopTTY()
{
	/* Restore the tty settings back to the original */
	tcsetattr(0, 0, &oldSettings);
}


void setCursor(int x, int y, struct TTY* tty)
{
	
}