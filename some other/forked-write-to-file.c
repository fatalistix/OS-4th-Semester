#include <stdio.h>
#include <unistd.h>


int main()
{
    FILE * f = fopen("testfile.txt", "w");
    int pid = fork();
    if (pid == 0)
    {
        sleep(3);
        fprintf(f, "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
    }
    fclose(f);
    return 0;
}
