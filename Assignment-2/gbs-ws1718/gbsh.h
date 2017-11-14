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
void prompt(UserInfo userInformation, char **command, char **directory);

/* It stores information of the current user in a proper struct
 * @Parameters: Struct UserInfo
 * @Return: Void, it just stores data on the UserInfo struct (user, host, cwd)
 */
void storeInfo (UserInfo *infoUser);

/* It shows the user's current path (pwd command)
 * @Parameters: None
 * @Return: Void, it just shows the current path to the user
 */
void pwd ();

/* It shows the user's current path (pwd command)
 * @Parameters: String: Directory
 * @Return: Void, it just shows all the files and sub-directories in the given folder
 */
void ls(char *folder);
/******************************************************************************/
