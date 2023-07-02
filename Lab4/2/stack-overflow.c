#include <stdio.h>
#include <unistd.h>

void makeMeOverflow()
{
    char buffer[4096];
    printf("Here\n");
    usleep(500000);
    makeMeOverflow();
}


int main(int argc, char * argv[])
{
    printf("<==>PID=%d<==>\n", getpid());
    sleep(10);
    
    makeMeOverflow();

    return 0;
}
