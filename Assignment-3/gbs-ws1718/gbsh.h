/**************************** Libraries ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h> // For working with directory entries
/******************************************************************************/

/***************************** Structs ****************************************/
typedef struct UserInformation {
    char *user;
    char *host;
    char *cwd;
} UserInfo;
/******************************************************************************/

/************************ Prototypes of used functions ************************/

/* Prompt that waits for an user input
 * @Parameters: UserInfo Struct: user, host and cwd information
                String command (followed by Enter)
 * @Return: Void, it just shows the statement + command to the user
 */
void prompt();

/* Parse command line arguments
 * @Parameters: cmd, command line input
                argc, number of arguments output
                argv, array of arguments output
 * @Return void
 */
void parse_cmd(char *cmd, int *argc, char ***argv, char **input_path, char **output_path);

/* Free the allocated memory form parse_cmd call
 *  @Parameters: argc, number of arguments
                 argv, arguments
 */
void free_cmd(int argc, char ***argv, char **input_path, char **output_path);

/* It stores information of the current user in a proper struct
 * @Parameters: Struct UserInfo
 * @Return: Void, it just stores data on the UserInfo struct (user, host, cwd)
 */
void storeInfo (UserInfo *infoUser);

/* It lists the contents of the directory specified by the given folder
 * @Parameters: String: folder
 * @Return: Void, it just shows all the files and sub-directories in the given folder
 */
void ls(char *folder, char *output);

/* It changes to the directory specified by the given folder.
 * @Parameters: String: folder
 * @Return: Void, it just changes the user's current folder
 */
void cd(char *folder);
/******************************************************************************/
