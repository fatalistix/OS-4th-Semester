#include <stdio.h>

int main(int argc, char * argv[])
{
    
    char arr[256];
    FILE * f = fopen("/proc/self/maps", "r");
    fseek(f, 10, SEEK_SET);
    fread(arr, 1, 30, f);
    for (int i = 0; i < 30; ++i)
    {
        printf("%c", arr[i]);
    }
    
    
    


    return 0;
}
