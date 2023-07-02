#include <stdio.h>
#include <unistd.h>

#include "./linearmalloc.h"


int main(int argc, char * argv[])
{
    size_t var = 0x4000;
    printf("var = %zu\n", var);
    size_t buf = var >> 8;
    printf("buf = %zu\n", buf);

//    printf("<==> PID=%d <==>\n", getpid());
//
//    char * a = linearmalloc(10);
//    char * b = linearmalloc(100);
//    char * c = linearmalloc(100);
//    printf("%p\n", a);
//    printf("%p\n", b);
//    printf("%p\n", c);
//
//    *a = 'h';
//    *(a + 1) = 'e';
//    *(a + 2) = 'l';
//    *(a + 3) = 'l';
//    *(a + 4) = '\0';
//
//    printf("%s\n", a);
//
//    linearfree(a);
//    linearfree(b);
//
//    char * d = linearmalloc(110);
//    printf("%p\n", d);
//
//    int buf;
//    scanf("%d", &buf);
    return 0;
}
