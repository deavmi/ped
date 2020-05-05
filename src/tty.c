#include<termios.h>
#include<sys/ioctl.h>

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


	testicle();
}

void testicle()
{
	/* window size struct */
	struct winsize size;
	
	/* IOCTL the vnode via the fd that points to it */
	ioctl(0, TIOCGWINSZ, &size);

	printf("row: %u\n", size.ws_row);
	printf("col: %u\n", size.ws_col);
}

void stopTTY()
{
	/* Restore the tty settings back to the original */
	tcsetattr(0, 0, &oldSettings);
}


void setCursor(int x, int y, struct TTY* tty)
{
	
}