#include "types.h"
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

/**
* Plugins
*
* Manages the plugin system.
*/

void loadPlugins(struct Editor* editor)
{
	/* Look through file ted.conf */
	int fd = open("ted.conf", O_RDWR);

	if(fd>=0)
	{
		
	}
	else
	{
		
	}
}

void addPlugin(struct Editor* editor, char* pluginName)
{
	
}