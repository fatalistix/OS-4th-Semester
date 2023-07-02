#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    char * buffer = (char *) malloc(sizeof(char) * 100);
    if (buffer == NULL)
    {
        char error[256] = ""; 
        perror(strcat(strcat(error, argv[0]), ": allocation error"));
        return errno;
    }

    strcpy(buffer, "Hello, World");
    printf("buffer = %s\n", buffer);


    free(buffer);


    printf("buffer = %s\n", buffer);
    
    char * buffer1 = (char *) malloc(sizeof(char) * 100);
    if (buffer1 == NULL)
    {
        char error[256] = "";
        perror(strcat(strcat(error, argv[0]), ": allocation error"));
        return errno;
    }

    strcpy(buffer1, "Hello, World!");
    printf("buffer1 = %s\n", buffer1);

    buffer1 += 50;

    free(buffer1);
    printf("buffer1 = %s\n", buffer1);

    return 0;
}
