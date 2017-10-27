/**************************** Libraries ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
void prompt(UserInfo userInformation,char **command);

/* Prompt that waits for an user input
 * @Parameters: Struct UserInfo
 * @Return: Void, it just stores data on the UserInfo struct (user, host, cwd)
 */
void storeInfo (UserInfo *infoUser);

/******************************************************************************/
