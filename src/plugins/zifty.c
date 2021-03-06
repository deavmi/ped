#include "../types.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/**
* Zifty
*
* Tristan's status line generator,
* is default as Tristan knows best.
*/

void dispatch(struct Session* session)
{
	/* Get dimensions */
	unsigned int columns = session->teletype->columns;

	/* General counter */
	unsigned int i = 0;

	/* STatus line */
	session->status = malloc(columns);
	*session->status=0;

	/* Add name */
	strcat(session->status, "🐻️️[");
	strcat(session->status, session->name);
	strcat(session->status, "]");

	strcat(session->status, "(");
	char* temp = malloc(20);
	sprintf(temp, "%u", session->size);
	strcat(session->status, temp);
	strcat(session->status, ")");
	
	i = 0;
	while(i < columns-strlen(session->name)-2-strlen(temp)-2-2)
	{
		strcat(session->status, "-");
		i++;
	}

	//free(temp);
}