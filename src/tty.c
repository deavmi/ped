#include<termios.h>

struct termios startTTY()
{
	/* First create a struct */
	struct termios termy;

	/* Get current attributes */
	tcgetattr(0, &termy);

	/* Save current tty settings for restoral later */
	struct termios oldTermy = termy;

	/* Set the tty input modes */
	//tcflag_t inputFlags = 0;
	termy.c_iflag = termy.c_iflag | IGNBRK;
	termy.c_lflag = termy.c_lflag & ~ISIG;
	termy.c_lflag = termy.c_lflag & ~ECHO;

	cfmakeraw(&termy);
	
	/* Set the tty to raw mode */
	tcsetattr(0, 0, &termy);

	return oldTermy;
}