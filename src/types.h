/**
* Represents the tty
*/
struct TTY
{
	/* Dimensions of tty */
	unsigned int rows;
	unsigned int columns;

	/* Current position */
	unsigned int cursorX;
	unsigned int cursorY;

	/* Screen data to draw */
	char* screen;

	/* Length of screen buffer */
	unsigned int screenLength;
};

/**
* Plugin
*
*/
struct Plugin
{
	//void (*dispatch)(struct Session*)
};

/**
* Represents an open editor session.
*
* Consists of the file descriptor
* associated with the open file
* and the size of the file (this
* is loaded once and changed accordingly
* with respect to the changes writing
* back to disk).
*/

struct Session
{
	/* Open file descriptor */
	int fd;

	/* Data pointer */
	char* data;

	/* Current size */
	unsigned int size;

	/* Position */
	unsigned int position;
	unsigned int fileX;
	unsigned int fileY;

	/* Whether or not we are running */
	char isActive;

	/* Pointer to tty object */
	struct TTY* teletype;

	char* name;

	char* status;
	unsigned int statusLength;
};