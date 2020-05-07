#include "types.h"

/**
* This takes the address of a Editor struct
* and sets up the initial values of it.
*/
void newEditor2(struct Editor* editor)
{
	/* Initialize `sessions` */
	editor->sessions = 0;

	/* Initialize `currentSession` */
	editor->currentSession = 0;

	/* Initialize `plguins` */
	editor->plugins = 0;
}

/**
* Sets the `currentSession` field to
* the address of the given struct Session.
*/
void setCurrentSession(struct Editor* editor, struct Session* selectedSession)
{
	/* Set the `currentSession` field */
	editor->currentSession = selectedSession;
}