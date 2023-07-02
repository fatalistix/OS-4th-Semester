#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: missing environment variable name\n", argv[0]);
        return 1;
    }

    char * envName = argv[1];
    char * envValue = getenv(envName);
    if (envValue == NULL)
    {
        fprintf(stderr, "%s: error getting env var: no variable with name \'%s\'\n", argv[0], argv[1]);
        return 1;
    }

    printf("%s = %s\n", envName, envValue);

    if (setenv(envName, "555", 1) != 0)
    {
        char error[256] = "";
        perror(strcat(strcat(error, argv[0]), ": cannot setenv"));
        return errno;
    }

    envValue = getenv(envName);
    if (envValue == NULL)
    {     
        fprintf(stderr, "%s: error getting env var: no variable with name \'%s\'\n", argv[0], argv[1]);
        return 1;
    }

    printf("%s = %s\n", envName, envValue);

    return 0;
}
