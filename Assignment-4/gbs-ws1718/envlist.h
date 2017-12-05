struct envvar{
    char envVarId[100]; // Id of the environment variable
    char envVarValue[100]; // Id of the environment variable
};
typedef struct envvar ENV_VAR;

struct nodeList{
    ENV_VAR data; // id and value of the env var
    struct nodeList *next; // Pointer to the next list's element
};
typedef struct nodeList NODE_LIST;

/* Function that initializes the list
 * @Parameters: Void
 * @Return: NULL
 */
NODE_LIST* initEnvironList(void);

/* Function that set the list of environment variables up with the shell path
 * @Parameters: NODE_LIST*: First node of the list of environment variables
 * @Return: NODE_LIST*: First node of the list of environment variables
 */
NODE_LIST* setUpShellEnv(NODE_LIST *FirstNode);

/* Function that add a new environment variable into the list of env vars
 * @Parameters: NODE_LIST*: First element (shell info), 
 *              ENV_VAR: Information about the the environment var (id and value)
 * @Return: NODE_LIST*: First node of the list of environment variable
 */
NODE_LIST* setEnviron(NODE_LIST *FirstNode, ENV_VAR dataEnviron);

/* Function that removes a new environment variable from the list of env vars
 * @Parameters: NODE_LIST*: First element (shell info), 
 *              ENV_VAR: Information about the the environment var (id and value)
 * @Return: NODE_LIST*: First node of the list of environment variable after the deletion
 */
NODE_LIST* unsetEnviron(NODE_LIST *FirstNode, char* varToBeDe);

/* Function that shows all the environment variables which have been set
 * @Parameters: NODE_LIST*: First element (shell info)
 * @Return: Int: Amount of set variables in the shell
 */
int showEnvironList(NODE_LIST *FirstNode, char *output);