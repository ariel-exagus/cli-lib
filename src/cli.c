/*
 * cli.c
 *
 *  Created on: 13 sept 2021
 *      Author: Ariel
 */


#include "../inc/cli.h"
//#include "cli.h"


static char command[MAX_COMMAND_SIZE];
static bool exit_command = false;
static bool isCommandReceived = false;
static uint8_t index = 0;
static bool commandTooLongFlag = false;

static void command_received(char *command_text);
static void reset_cmd(char *pArg);
static void reconnect_cmd(char *pArg);
//static void set_wifi_auth(char *ssid_pwd_auth);
static void get_cli_version(char *pArg);
static void get_firmware_version(char *pArg);
static void set_debug_level(char *pArg);
static void exit_cmd(char *pArg);

const char * const cli_version_number             = "0.0";
const char * const firmware_version_number        = "0.0.0";


struct cmd
{
    const char * const command;
    void (* const handler)(char *argument);
};

const struct cmd commands[] =
{
    { "reset",       reset_cmd},
    { "reconnect",   reconnect_cmd },
    { "cli_version", get_cli_version },
    { "version",     get_firmware_version },
    { "debug",       set_debug_level },
	{ "exit",        exit_cmd }
};

void CLI_init(void)
{
	printf(UNKNOWN_CMD_MSG);
}

static bool endOfLineTest(char c)
{
   static char test = 0;
   bool retvalue = true;

   if(c == '\n')
   {
      if(test == '\r')
      {
         retvalue = false;
      }
   }
   test = c;
   return retvalue;
}

uint32_t CLI_task()
{
    bool cmd_rcvd = false;
	char c = 0;
   // read all the EUSART bytes in the queue
   while(!isCommandReceived && !cmd_rcvd)
   {
      c = getchar();
      putchar(c);
      // read until we get a newline
      if(c == '\r' || c == '\n')
      {
         command[index] = 0;

         if(!commandTooLongFlag)
         {
            if( endOfLineTest(c) && !cmd_rcvd )
            {
               command_received((char*)command);
			   cmd_rcvd = true;
            }
         }
         if(commandTooLongFlag)
         {
            printf(NEWLINE"Command too long"NEWLINE);
         }
         index = 0;
         commandTooLongFlag = false;
      }
      else // otherwise store the character
      {
         if(index < MAX_COMMAND_SIZE)
         {
            command[index++] = c;
         }
         else
         {
            commandTooLongFlag = true;
         }
      }
   }

   return CLI_TASK_INTERVAL;
}

//static void set_wifi_auth(char *ssid_pwd_auth)
//{
//    char *credentials[3];
//    char *pch;
//    uint8_t params = 0;
//	uint8_t i;
//
//    for(i=0;i<=2;i++)credentials[i]='\0';
//
//    pch = strtok (ssid_pwd_auth, ",");
//    credentials[0]=pch;
//
//    while (pch != NULL && params <= 2)
//    {
//        credentials[params] = pch;
//        params++;
//        pch = strtok (NULL, ",");
//
//    }
//
//    if(credentials[0]!=NULL)
//    {
//        if(credentials[1]==NULL && credentials[2]==NULL) params=1;
//        else if(credentials[1]!= NULL && credentials[2]== NULL)
//        {
//            params=atoi(credentials[1]);//Resuse the same variable to store the auth type
//            if (params==2 || params==3)params=5;
//            else if(params==1);
//            else params=2;
//        }
//		else params = atoi(credentials[2]);
//    }
//
//    switch (params)
//    {
//        case WIFI_PARAMS_OPEN:
//                strncpy(ssid, credentials[0],MAX_WIFI_CREDENTIALS_LENGTH-1);
//                strcpy(pass, "\0");
//                strcpy(authType, "1");
//            break;
//
//        case WIFI_PARAMS_PSK:
//		case WIFI_PARAMS_WEP:
//                strncpy(ssid, credentials[0],MAX_WIFI_CREDENTIALS_LENGTH-1);
//                strncpy(pass, credentials[1],MAX_WIFI_CREDENTIALS_LENGTH-1);
//                sprintf(authType, "%d", params);
//            break;
//
//        default:
//			params = 0;
//            break;
//    }
//	if (params)
//	{
//		printf("OK\r\n\4");
//        if(CLOUD_isConnected())
//        {
//            MQTT_Close(MQTT_GetClientConnectionInfo());
//        }
//		wifi_disconnectFromAp();
//	else
//	{
//		printf("Error. Wi-Fi command format is wifi <ssid>[,<pass>,[authType]]\r\n\4");
//	}
//}

static void reconnect_cmd(char *pArg)
{
    (void)pArg;

    printf("OK\r\n");
}

static void reset_cmd(char *pArg)
{
	(void)pArg;

	printf("Reset Command\r\n");
//	wdt_enable(WDTO_30MS);
//	while(1) {};
}

static void set_debug_level(char *pArg)
{
   int level = DEBUG_LEVEL_DEFAULT;
   if(*pArg >= '0' && *pArg <= '4')
   {
      level = *pArg - '0';
      //debug_setSeverity(level);
      printf("OK\r\n");
   }
   else
   {
      printf("debug parameter must be between 0 (Least) and 4 (Most) \r\n");
   }
}


static void get_cli_version(char *pArg)
{
    (void)pArg;
    printf("v%s\r\n", cli_version_number);
}

static void get_firmware_version(char *pArg)
{
    (void)pArg;
    printf("v%s\r\n", firmware_version_number);
}

static void exit_cmd(char *pArg)
{
	(void)pArg;
	printf("Bye bye\r\n");
	exit_command = true;
}

static void command_received(char *command_text)
{
    char *argument = strstr(command_text, " ");
    uint8_t cmp;
    uint8_t ct_len;
    uint8_t cc_len;
	uint8_t i = 0;

    if (argument != NULL)
    {
        /* Replace the delimiter with string terminator */
        *argument = '\0';
        /* Point after the string terminator, to the actual string */
        argument++;
    }

    for (i = 0; i < sizeof(commands)/sizeof(*commands); i++)
    {
        cmp = strcmp(command_text, commands[i].command);
        ct_len = strlen(command_text);
        cc_len = strlen(commands[i].command);

        if (cmp == 0 && ct_len == cc_len)
        {
            if (commands[i].handler != NULL)
            {
                commands[i].handler(argument);
                return;
            }
        }
    }

    printf(UNKNOWN_CMD_MSG);
}



