/*
 * commands.c
 *
 *  Created on: Jul 28, 2023
 *      Author: nivetha_anbuselvan
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "macro.h"
#include "main.h"
#include "version.h"
#include "actuator.h"
#include "debugCli.h"
#include "commands.h"

// The definition of the list of commands.
static CLI_Command_Definition_t Commands[]= {
	/*"COMMAND" , "ID" ,  COMMAND CALLBACK ,   NUMBER OF INPUT PARAMETERS,PERMISSIONS */
	{"help"     , HELP_CLI    ,help_callback      , PARAM_SIZE_0                 , PERMISSION_LEVEL_0 },	/* The first command in the list is always the help command, defined in this file. */
	{"swinfo"   , SWINFO_CLI    ,callback_swinfo    , PARAM_SIZE_1                 , PERMISSION_LEVEL_0},
	{"gpio"     , GPIO_CLI    ,callback_gpio      , PARAM_SIZE_3				  , PERMISSION_LEVEL_0},
	{"testall"  , TEST_ALL_CLI   ,callback_testall   , PARAM_SIZE_0				  , PERMISSION_LEVEL_0},
	{" "      ,MAXIMUM_CLI,		NULL			,PARAM_SIZE_0				  ,PERMISSION_LEVEL_0}

};

static void TestAllPeripheral(void);

//! This function process the command received through diagnostic mode.
//!
//! \param[in] cptrBuffer    Pointer to the cli command buffer
//! \param[out] none
//!
//! \return none
//!
void ProcessDiagnosticCommand(char * cptrBuffer)
{
    bool bCliProcessReturn;
    char cOutputString[ CLI_MAX_OUTPUT_LENGTH ];
    char cCommandBuffer[CLI_MAX_COMMAND_LENGTH];
    int argc = SET_TO_ZERO;
    char *argv[12];

    argc = SET_TO_ZERO;
    argv[INDEX_0] = SET_TO_ZERO;

    if(NULL_POINTER != cptrBuffer)
    {
        strncpy((char *)cCommandBuffer, (const char *)cptrBuffer, sizeof (cCommandBuffer));
        strncpy((char *)cOutputString, (const char *)cptrBuffer, sizeof (cOutputString));
        memset(argv, INIT_STRING_NULL, sizeof (argv));

        // Parse command and parameter strings/values from command string
     //  get_arguments(cCommandBuffer, &argc,(char **) argv);

        char * cptrCurrentargument = strtok(cCommandBuffer," \t");
             while(cptrCurrentargument != NULL)
           	 {
                    argv[argc++] = cptrCurrentargument;
           	     cptrCurrentargument = strtok(NULL," ");
           	 }


        /* Check the entered command with already registered commands and execute */
        if (SET_TO_ZERO != strcmp(argv[INDEX_0], ""))
        {
            do {
                /*Command string process with below function and out value return back with caOutputString*/
                bCliProcessReturn = CLIProcessCommand(
                        cOutputString, /* The output buffer string of command output. */
                        CLI_MAX_OUTPUT_LENGTH, /* The maximum size of the output buffer. */
                        argc, /* No of parameter for given command */
                        argv /* pointer to hold the command string */
                        );

                /* Send the ouput to uart console */
                debugText (cOutputString);
                debugText ("\r\n");

                if (false == bCliProcessReturn) {
                    bCliProcessReturn = SET_TO_ZERO;
                    break;
                }
            } while (bCliProcessReturn != FAILURE);

            /* Clear the output buffer */
            memset(cOutputString, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH);
            /* Clear the input buffer */
            memset(cCommandBuffer, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH);
            /* send the executed command output to uart console */
            debugText ("\r\n");
        }
        else
        {
            /* Clear the output buffer */
            memset(cOutputString, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH);
            /* Clear the input buffer */
            memset(cCommandBuffer, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH);


            /* send the executed command output to uart console */
            debugText ("\r\n");
        }
    }
}


/** @fn long CLIProcessCommand( int8_t * pcWriteBuffer, size_t xWriteBufferLen,int argc, char * * argv)
 *  @brief 	Process the each command
 *              CLIProcessCommand should be called repeatedly until it returns false.
 *  @param
 *  uint8_t *pcWriteBuffer              - Points to the first byte of debug message or error message
 *  uint8_t xWriteBufferLen             - is the value of write buffer length
 *  int argc                            - is the number of strings pointed to by argv
 *  char * *argv                        - is a pointer to the arguments.
 *
 *  @return : xReturn - False - Command not found
 *  @pre                True  - Success
 *  @invariant
 *  @post
 */
int8_t CLIProcessCommand( char * cptrWriteBuffer, uint8_t u8WriteBufferLen, int argc, char **argv)
{
    int8_t s8Return = true;
	const char *cptrRegisteredCommandString;
	uint8_t u8CommandStringLength;
    char cCommandInput[BUFFER_SIZE_128];
    CLiCommandList_t CommandId;

    strncpy(cCommandInput, cptrWriteBuffer, sizeof(cCommandInput));

	/* Note:  This function is not re-entrant.  It must not be called from more
	thank one task. */

        /* Search for the command string in the list of registered commands. */
      //  for (strptrCommandList = &gstrRegisteredCommands; strptrCommandList != NULL; strptrCommandList = strptrCommandList->pxNext)
    	for(CommandId = HELP_CLI;CommandId < MAXIMUM_CLI;CommandId++)
        {
            cptrRegisteredCommandString = Commands[CommandId].cptrCommand;
            u8CommandStringLength = strlen(cptrRegisteredCommandString);

            /* To ensure the string lengths match exactly, so as not to pick up
            a sub-string of a longer command, check the byte after the expected
            end of the string is either the end of the string or a space before
            a parameter. */
            if ((cCommandInput[ u8CommandStringLength ] == ' ') || (cCommandInput[ u8CommandStringLength ] == INIT_STRING_NULL))
            {
                if (SET_TO_ZERO == strncasecmp(cCommandInput, cptrRegisteredCommandString, u8CommandStringLength))
                {
                    /* The command has been found.  Check it has the expected
                    number of parameters.  If cExpectedNumberOfParameters is -1,
                    then there could be a variable number of parameters and no
                    check is made. */
                    if (Commands[CommandId].s8ExpectedNumberOfParameters >= SET_TO_ZERO)
                    {
                        if (strcasecmp(argv[INDEX_0], "help")) //if not help get in
                        {

                            if (get_number_of_parameters(cCommandInput) != Commands[CommandId].s8ExpectedNumberOfParameters)
                            {
                                s8Return = false;
                            }
                        }
                    }

                    break;
                }
            }
        }


    if ((CommandId < MAXIMUM_CLI ) && (s8Return == false)) {
        /* The command was found, but the number of parameters with the command
        was incorrect. */
        strncpy(cptrWriteBuffer, "\r\nIncorrect command parameter(s). "
                "Enter \"help 'Command'\" to view a Usage.\r\n", u8WriteBufferLen);

    } else if (CommandId < MAXIMUM_CLI) {
        memset(cptrWriteBuffer, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH);
        /* Call the callback function that is registered to this command. */
        s8Return = Commands[CommandId].pxCommandInterpreter(
                argc, argv);


        /* If xReturn is pdFALSE, then no further strings will be returned
        after this one, and	pxCommand can be reset to NULL ready to search
        for the next entered command. */
        if (s8Return == false) {
        	CommandId = MAXIMUM_CLI+1;
        }
    } else {
        /* pxCommand was NULL, the command was not found. */
        strncpy(cptrWriteBuffer, (const char *) "\r\nCommand not recognized. "
                "Enter \"help\" to view a list of available commands.\r\n", u8WriteBufferLen);
        s8Return = false;
    }

    return s8Return;
}

/** @fn static int8_t getNumberOfParameters( const int8_t * pcProcessString )
 *  @brief 	get the number of parameter entered in console
 *          get the number of parameter entered in console
 *
 *  @param :
 *  const int8_t * pcProcessString - Points to the string
 *
 *  @return : ciParameters -The value returned is one less than the number of space delimited words
 *  @pre
 *  @invariant
 *  @post
 */
uint8_t get_number_of_parameters( const char * cptrProcessString )
{
	uint8_t numberofParameters = SET_TO_ZERO;
	char cCommandCopy[BUFFER_SIZE_128];
	/* Count the number of space delimited words in pcProcessString. */
	 strncpy(cCommandCopy, cptrProcessString, sizeof(cCommandCopy));
	char * token = strtok(cCommandCopy," ");

	 while(token != NULL)
	 {
         token = strtok(NULL," ");
	     numberofParameters++;
	  }
	 //! remove the last null value parameter
	 numberofParameters--;

	//! The value returned is one less than the number of space delimited words,
    //! as the first word should be the command itself. */
	return numberofParameters;
}

////////////////////////////////////////////////////////////////////////////////
/// CLI CALLBACK FUNCTION DEFINITION
////////////////////////////////////////////////////////////////////////////////
/** @fn static long prvHelpCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen,  int argc,char * *argv )
 *  @brief 	Execute the help command
 *
 *  @param
 *  int argc                         - is the number of strings pointed to by argv
 *  char * *argv                     - is a pointer to the arguments.
 *
 *  @return : xReturn - False - No more command in the list
 *  @pre                True  - Commands are available
 *  @invariant
 *  @post
 */
int8_t help_callback(int argc,char * *argv )
{

    int8_t s8Return;
    char data[BUFFER_SIZE_128];
    CLiCommandList_t CommandIdList;
    //! For removing compiler warning
    (void) argc;


        /* Reset the pxCommand pointer back to the start of the list. */
    CommandIdList = HELP_CLI;

    if (argc >= 2)
    {
        /* Process the help command for individual Module */
    	 while(Commands[CommandIdList].commandID <= MAXIMUM_CLI)
    	        {
    		 	 	 //! if command is not found
    		          if(Commands[CommandIdList].commandID == MAXIMUM_CLI)
    		           {
    		               debugText( "\n\r Incorrect command Parameter!! ");
    		                return false;
    		            }
    		          if ( (strcmp(argv[INDEX_1],(const char *)Commands[CommandIdList].cptrCommand) == SET_TO_ZERO) )
    		              {
    		        	  if ( PERMISSION_LEVEL_0 <= (TEPermissionLevel)Commands[CommandIdList].ePermissionLevel )
    		        	       {
    		        		  	  	  sprintf(data, "\r\n\r\n Command: \r\n\t%s",(const char *)Commands[CommandIdList].cptrCommand);
    		        		      	  debugText(data);
    		        		  	  	  switch(Commands[CommandIdList].commandID)
    		        		  	  	  {
    		        		  	  	  case 0:
    		        		  	  	  {
    		        		  	  		debugText( "\r\n\r\n Description:Lists all the registered commands \r\n");
    		        		  	  	    debugText( "\r\n\r\n Usage:help <command name> \r\n\t");

    		        		  	  	  }break;
    		        		  	  	  case 2:
    		        		  	  	  {
    		        		  	  		debugText( "\r\n\r\n Description:Get Software version \r\n");
    		        		  	  		debugText( "\r\n\r\n Usage: gpio out <port> <pin> \r\n\t");
    		        		  	  	    debugText(  "gpio in <port> <pin> \r\n\t");
    		        		  	  		debugText( "gpio set <port> <pin> \r\n\t");
    		        		  	  	    debugText( "gpio clr <port> <pin> \r\n\t");
    		        		  	  	    debugText( "gpio get <port> <pin> \n\r\t ");
    		        		  	  	  }break;
    		        		  	  	  case 1:
    		        		  	  	  {
    		        		  	  		debugText( "\r\n\r\n Description:This command used to configure GPIO. \r\n");
    		        		  	  		debugText( "\r\n\r\n Usage: swinfo read \r\n\t");
    		        		  	  	  }break;
    		        		  	  	  case 3:
    		        		  	  	  {
    		        		  	  		debugText( "\r\n\r\n Description:Test all peripheral \r\n");
    		        		  	  		debugText( "\r\n\r\n Usage: testall\r\n\t");
    		        		  	  	  }break;

    		        		  	  	  default:
    		        		  	  	  {

    		        		  	  		 debugText( "\r\n\r\n  NO such Command: \r\n\t");
    		        		  	  		 debugText( "Enter \"help\" to view a list of available commands.\r\n");

    		        		  	  	  }break;
    		        		  	  	  }
    		        	       }
    		        	  else
    		        	       {
    		        	          debugText( "\n\r Permission Denied!! ");
    		        	       }
    		        	        return true;
    		              }


    		          CommandIdList++;

    	         }



     }

    debugText( "\r\n");

    while (Commands[CommandIdList].commandID < MAXIMUM_CLI)
    {
        if(PERMISSION_LEVEL_0 <= Commands[CommandIdList].ePermissionLevel)
        {
        	sprintf( data,"\r\n %-16s ",Commands[CommandIdList].cptrCommand);
            debugText(data);

        }
        CommandIdList++;
    }

    if (CommandIdList == MAXIMUM_CLI) {
        /* There are no more commands in the list, so there will be no more
        strings to return after this one and pdFALSE should be returned. */
        s8Return = false;
    }

    debugText( "\r\n");

    return s8Return;
}
//! This is diagnostic "swinfo" command callback function
//!
//! \param[in] argc     Argument Count
//! \param[in] argv     Argument Value
//! \pram[out] none
//!
//! \return true
//!         FALSE
//!
int8_t callback_swinfo(int argc, char **argv)
{
    int8_t s8RetStatus = false;
    char version_data[BUFFER_SIZE_128];

    if(argc > INDEX_1)
    {
        if(!(strcmp(argv[INDEX_1],DIAG_READ_COMMAND)))
        {
        	 sprintf(version_data,"\r\n Version: %s\r\n", VERSION);
        	 debugText(version_data);
        }
        else
        {
            debugText( "\r\n Invalid Command\r\n");
        }
    }

    return s8RetStatus;
}

//! This is diagnostic "gpio" command callback function
//!
//! \param[in] argc     Argument Count
//! \param[in] argv     Argument Value
//! \pram[out] none
//!
//! \return true
//!         FALSE
//!
int8_t callback_gpio(int argc, char **argv)
{

	debugText("Check GPIO Pins");
	 int8_t s8RetStatus = false;
	 uint8_t u8PortId = SET_TO_ZERO;
    uint8_t u8PinBit = SET_TO_ZERO;



  if(argc > INDEX_3)
    {


         u8PortId = atoi (argv[INDEX_2]);
         u8PinBit = atoi (argv[INDEX_3]);

		if (!(strcmp(argv[INDEX_1], "out"))) {


			gpiooperate(u8PortId, u8PinBit, SET_OUT);

		} else if (!(strcmp(argv[INDEX_1], "in"))) {

			gpiooperate(u8PortId, u8PinBit, SET_IN);
		} else if (!(strcmp(argv[INDEX_1], "set"))) {
			gpiooperate(u8PortId, u8PinBit, GPIO_SET);
		} else if (!(strcmp(argv[INDEX_1], "clr"))) {
			gpiooperate(u8PortId, u8PinBit, GPIO_RESET);
		}
		 else if (!(strcmp(argv[INDEX_1], "get"))) {
			gpiooperate(u8PortId, u8PinBit, GET);
		}
		 else
		{
			debugText("\r\n Invalid GPIO command.");
		}



    }

    return s8RetStatus;
}

//! This is diagnostic "testall" command callback function
//!
//! \param[in] argc     Argument Count
//! \param[in] argv     Argument Value
//! \pram[out] none
//!
//! \return true
//!         FALSE
//!
int8_t callback_testall(int argc, char **argv)
{
    int8_t s8RetStatus = false;

    TestAllPeripheral();

    return s8RetStatus;
}


//! This function will test all peripherals of the device and prints the report.
//!
//! \param[in] none
//! \param[out] none
//!
//! \return none
//!
static void TestAllPeripheral(void)
{
	 debugText( "\r\n ************************************************\r\n");
	 debugText( "\r\n                    TEST RESULTS                 \r\n");
	 debugText( "\r\n ************************************************\r\n");
}
