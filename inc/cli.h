/*
 * cli.h
 *
 *  Created on: 13 sept 2021
 *      Author: Ariel
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_COMMAND_SIZE        100
#define NEWLINE "\r\n"

#define UNKNOWN_CMD_MSG "--------------------------------------------" NEWLINE\
                        "Unknown command. List of available commands:" NEWLINE\
                        "reset"NEWLINE\
                        "reconnect" NEWLINE\
                        "version" NEWLINE\
                        "cli_version" NEWLINE\
                        "wifi <ssid>[,<pass>,[authType]]" NEWLINE\
                        "debug" NEWLINE\
						"exit" NEWLINE\
                        "--------------------------------------------"NEWLINE //"\4"

#define CLI_TASK_INTERVAL      50
#define DEBUG_LEVEL_DEFAULT		0


void CLI_init(void);
uint32_t CLI_task();
//void CLI_setdeviceId(char *);

#endif /* INC_CLI_H_ */
