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

/* Function that add a new environment variable into the list of env vars
 * @Parameters: NODE_LIST: First element (shell info), 
 *              ENV_VAR: Information about the the environment var (id and value)
 * @Return: First node of the list of environment variable
 */
NODE_LIST* setEnviron(NODE_LIST *FirstNode, ENV_VAR dataEnviron);

/* Function that shows all the environment variables which have been set
 * @Parameters: NODE_LIST: First element (shell info)
 * @Return: Int: Amount of set variables in the shell
 */
int showEnvironList(NODE_LIST *FirstNode);