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

	char isActive;
};