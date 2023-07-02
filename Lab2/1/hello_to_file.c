#include <stdio.h>

int main() 
{
	FILE * fp;
	fp = fopen("out.txt", "w");

	fprintf(fp, "Hello, World!\n");
	return 0;
}
