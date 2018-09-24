/* Experimentation code for stack.c */

/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//using namespace std;

int bof(char *str)
{
    char buffer[24];

    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);
    printf("The buffer is stored at: %p\n", &buffer);
    printf("At buffer+32 we have: %p\n", *(&buffer+32));

    return 1;
}
int main(int argc, char **argv)
{
    char str[517];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 517, badfile);
    bof(str);

    printf("Returned Properly\n");
    return 1;
}

