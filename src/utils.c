#include<string.h>
#include<unistd.h>

/**
* Writes a null-terminated string to the tty
*/
void writeStr(char* ntString)
{
	write(0, ntString, strlen(ntString));
}

/**
* Writes bytes from buffer `string` of length
* `length` to the tty
*/
void output(char* string, unsigned int length)
{
	write(0, string, length);
}

/**
* Reads and returns the single byte
* read from the tty.
*/
char getChar()
{
	char character;
	int b = read(0, &character, 1);
	return character;
}
