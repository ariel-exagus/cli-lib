/*
 * main.c
 *
 *  Created on: 18 sept 2021
 *      Author: Ariel
 */

#include <stdio.h>
#include "inc/cli.h"


int main()
{
	printf("---- Command Line Example ------\r\n");

	CLI_init();

	while(1)
	{
		CLI_task();
	}
	return 0;
}

