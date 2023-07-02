#include <stdio.h>
#include <unistd.h>

int myGlobalVariableInit = 217;

int myGlobalVariableNotInit;

int myGlobalConstInit = 309;


size_t createLocalAndInit()
{
    int localInFoo = 2004;
    return (size_t) &localInFoo;
}


int main()
{
    int myLocalVariableInit = 343;

    static int myStaticVariableInit = 685;

    const int myLocalConstInit = 676;

    printf("<==========>PID = %d<==========>\n", getpid());
    printf("Local  variable = %p\n", &myLocalVariableInit);
    printf("Static variable = %p\n", &myStaticVariableInit);
    printf("Local  const    = %p\n", &myLocalConstInit);
    printf("Global init     = %p\n", &myGlobalVariableInit);
    printf("Global notinit  = %p\n", &myGlobalVariableNotInit);
    printf("Global const    = %p\n", &myGlobalConstInit);

    printf("<==========><======><=========>\n");
    printf("Local in foo    = %p\n", (void *) createLocalAndInit());

    void * stackPointer;
    __asm__ volatile ("movq %%rsp, %0":"=r"(stackPointer));

    printf("Cur stack point = %p\n", stackPointer);

    scanf("%d", &myLocalVariableInit);
    

    return 0;
}
