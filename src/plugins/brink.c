#include "../types.h"
#include<stdio.h>

void dispatch(struct Session* session)
{
	//session->position = 5;
	int g = 1;
	g++;
	printf("EXAAAAACTLY!\n");
	session->isActive=0;

}