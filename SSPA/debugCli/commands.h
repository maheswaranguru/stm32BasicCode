/*
 * commands.h
 *
 *  Created on: Jul 28, 2023
 *      Author: nivetha_anbuselvan
 */

#ifndef SRC_SSPA_DEBUGCLI_COMMANDS_H_
#define SRC_SSPA_DEBUGCLI_COMMANDS_H_

#define BUFFER_SIZE_128                     128
#define COMMAND_INT_MAX_OUTPUT_SIZE         1024
#define CLI_MAX_SHELL_COMMAND_LENGTH        64
#define HISTORY_SIZE                        32
#define CLI_MAX_OUTPUT_LENGTH               128
#define CLI_MAX_COMMAND_LENGTH              128

#define DIAG_READ_COMMAND               "read"
#define DIAG_WRITE_COMMAND              "write"

#define PARAM_SIZE_VAR                      (-1)
#define PARAM_SIZE_0                        (0)
#define PARAM_SIZE_1                        (1)
#define PARAM_SIZE_2                        2
#define PARAM_SIZE_3                        3
#define PARAM_SIZE_4                        4
#define PARAM_SIZE_5                        5
#define PARAM_SIZE_6                        6
#define PARAM_SIZE_7                        7
#define PARAM_SIZE_8                        8

//! Special Characters
#define BACKSPACE                           '\b'
#define CARRIAGE_RETURN                     '\r'
#define NEW_LINE                            '\n'
#define TAB_CHAR                            '\t'





////////////////////////////////////////////////////////////////////////////////
// TYPEDEF ENUMIRATORS
////////////////////////////////////////////////////////////////////////////////


typedef enum{
	HELP_CLI,
	SWINFO_CLI,
	GPIO_CLI,
	TEST_ALL_CLI,
	MAXIMUM_CLI
}CLiCommandList_t;


//! CLI Command Permission Level
typedef enum
{
    PERMISSION_LEVEL_0 = 0,
    PERMISSION_LEVEL_1,
    PERMISSION_LEVEL_2,
    PERMISSION_LEVEL_3
}TEPermissionLevel;



//! The prototype to which callback functions used to process command line
//! commands must comply.  pcWriteBuffer is a buffer into which the output from
//! executing the command can be written, xWriteBufferLen is the ullength, in
//! bytes of the pcWriteBuffer buffer, and pcCommandString is the entire string
//! as input by the user (from which parameters can be extracted).
typedef int8_t (*pdCOMMAND_LINE_CALLBACK)(int argc, char **argv );

typedef struct xCOMMAND_LINE_INPUT
{
    //! The command that causes pxCommandInterpreter to be executed.  For example
    //! "help". Must be all lower case.
    const char * const cptrCommand;
    //! Identifier for the command
    CLiCommandList_t commandID;
    //! String that describes how to use the command.Should start with the command
    //! itself,and end with "\r\n".For example "help: Returns a list of all the commands\r\n".
    const pdCOMMAND_LINE_CALLBACK pxCommandInterpreter;
    //! Commands expect a fixed number of parameters, which may be zero.
    int8_t s8ExpectedNumberOfParameters;
    //! Necessary permission level to execute the command
    TEPermissionLevel ePermissionLevel;
} CLI_Command_Definition_t;

//! Structure to hold all command list
typedef struct xCOMMAND_INPUT_LIST
{
	const CLI_Command_Definition_t *strptrCommandLineDefinition;
	struct xCOMMAND_INPUT_LIST *pxNext;
} CLI_Definition_List_Item_t;

void diagnostic_int(void );
void ProcessDiagnosticCommand(char * cptrBuffer);
int8_t CLIProcessCommand( char * cptrWriteBuffer, uint8_t u8WriteBufferLen, int argc, char **argv);
int8_t cli_register_command( const CLI_Command_Definition_t * const strptrCommandToRegister );
uint8_t get_number_of_parameters( const char * cptrProcessString );
int8_t help_callback(int argc,char * *argv );
int8_t callback_swinfo(int argc, char **argv);
int8_t callback_gpio(int argc, char **argv);
int8_t callback_testall(int argc, char **argv);

#endif /* SRC_SSPA_DEBUGCLI_COMMANDS_H_ */
