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
